#include <iostream>
using namespace std;

#include <list>
#include <map>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#include "log.h"
#include "capwap_state.h"
#include "server.h"
#include "ap_list.h"
#include "buffer.h"
#include "capwap.h"
#include "business.h"

typedef int (*read_cb)(struct ap_dev *ap, CBuffer &buffer);
static read_cb read_cbs[] = {
    /*[CAPWAP_STATE_START] =*/ capwap_state_start,
    /*[CAPWAP_STATE_IDLE] = */ capwap_state_idle,
    /*[CAPWAP_STATE_SULKING] = */capwap_state_sulking,
    /*[CAPWAP_STATE_DISCOVERY] = */capwap_state_discovery,
    /*[CAPWAP_STATE_JOIN] = */capwap_state_join,
    /*[CAPWAP_STATE_IMAGE_DATA] = */capwap_state_image_data,
    /*[CAPWAP_STATE_CONFIGURE] = */capwap_state_configure,
    /*[CAPWAP_STATE_DATA_CHECK] = */capwap_state_data_check,
    /*[CAPWAP_STATE_RUN] = */capwap_state_run,
    /*[CAPWAP_STATE_RESET] = */capwap_state_reset,
    /*[CAPWAP_STATE_QUIT] = */capwap_state_quit,
    /*[CAPWAP_STATE_INVALID] = */NULL,
};


const char *capwap_state_string[CAPWAP_STATE_MAX] = {
    "START",
    "IDLE",
    "SULKING",
    "DISCOVERY",
    "JOIN",
    "IMAGE DATA",
    "CONFIGURE",
    "DATA CHECK",
    "RUN",
    "RESET",
    "QUIT",
    "UNKNOWN",
};

static bool capwap_packet_check(CCapwapHeader *cwheader, int state)
{
    switch(state)
    {
    case CAPWAP_STATE_DISCOVERY:
        return CAPWAP_PACKET_TYPE_DISCOVERY_REQ == capwap_packet_type(cwheader);
    case CAPWAP_STATE_JOIN:
        return CAPWAP_PACKET_TYPE_JOIN_REQ == capwap_packet_type(cwheader);
    case CAPWAP_STATE_CONFIGURE:
        return CAPWAP_PACKET_TYPE_CONFIG_STATUS_REQ == capwap_packet_type(cwheader);
    case CAPWAP_STATE_DATA_CHECK:
        return CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_REQ == capwap_packet_type(cwheader);
    case CAPWAP_STATE_RUN:
        return (CAPWAP_PACKET_TYPE_DATA_TRANSFER_REQ == capwap_packet_type(cwheader));
    default:
        dlog(LOG_ERR, "%s.%d error state %d packet type %d",
             __FILE__, __LINE__, state, capwap_packet_type(cwheader));
        break;
    }
    return false;
}

static CCapwapHeader * capwap_process_header(CBuffer &buffer, struct ap_dev *ap)
{
    CCapwapHeader cwheader;
    CCapwapHeader *ret_capwap_header = NULL;

    cwheader.Parse(buffer);

    if (!capwap_packet_check(&cwheader, ap->state))
    {
        dlog(LOG_ERR, "Packet from %#x is error, type %d state %s",
               ap->cl->peer_addr.sin_addr.s_addr,
               capwap_packet_type(&cwheader),
               capwap_state_string[ap->state]);
        // ap->state = CAPWAP_STATE_QUIT;
        return NULL;
    }

    // 解析完头部后，使用接口获取对应的报文类
    ret_capwap_header = capwap_get_packet(capwap_packet_type(&cwheader));

    // 设置头部数据不用重复解析
    if (ret_capwap_header) ret_capwap_header->setHeader(&cwheader);

    return ret_capwap_header;
}

int capwap_read_cb(struct client *cl, char *buf, size_t len)
{
    int ret = CAPWAP_MESSAGE_SAVE;
    do {
        if (NULL == cl->ap)
        {
            cl->ap = (struct ap_dev *)malloc(sizeof(struct ap_dev));
            bzero(cl->ap, sizeof(struct ap_dev));
            cl->ap->cl = cl;
            cl->ap->state = CAPWAP_STATE_START;
        }

        CBuffer buffer;
        buffer.toBuffer(buf, len);

        ret = read_cbs[cl->ap->state](cl->ap, buffer);

    }while ((ret != CAPWAP_MESSAGE_DONE) &&
            (cl->ap->state != CAPWAP_STATE_QUIT));

    if (CAPWAP_STATE_QUIT == cl->ap->state)
    {
        // 删除定时器
        uloop_timeout_cancel(&cl->timeout);

        cl->close_client_cb(cl);
    }

    return 0;
}
int capwap_state_change_cb(struct client *cl)
{
    // TODO 更新数据库相关数据，离线时间
    SAFE_FREE(cl->ap); 
    return 0;
}

int capwap_state_start(struct ap_dev *ap, CBuffer &buffer)
{
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    ap->state ++;
    return CAPWAP_MESSAGE_SAVE;
}
int capwap_state_idle(struct ap_dev *ap, CBuffer &buffer)
{
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    ap->state ++;
    return CAPWAP_MESSAGE_SAVE;
}
int capwap_state_sulking(struct ap_dev *ap, CBuffer &buffer)
{
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    ap->state ++;
    return CAPWAP_MESSAGE_SAVE;
}
int capwap_state_discovery(struct ap_dev *ap, CBuffer &buffer)
{
    CCapwapHeader *preq = capwap_process_header(buffer, ap);
    string str = "";
    CBusiness business;

    uloop_timeout_cancel(&ap->cl->timeout);

    if (NULL == preq)
    {
        ap->state = CAPWAP_STATE_QUIT;
        return CAPWAP_MESSAGE_DONE;
    }

    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    preq->Parse(buffer); //解析除头部以外的内容
    preq->SaveTo(str);

    dlog(LOG_DEBUG, "string = %s", str.c_str());

    business.set_business_type(CAPWAP_BUSINESS_DISCOVERY);
    business.set_business_string(str);

    if (business.Process(ap) != BUSINESS_SUCCESS)
    {
        ap->state = CAPWAP_STATE_QUIT;
    }
    else
    {
        ap->state = CAPWAP_STATE_JOIN;
        uloop_timeout_set(&ap->cl->timeout, CAPWAP_JOIN_TIMEOUT);
    }

    SAFE_DELETE(preq);
    return CAPWAP_MESSAGE_DONE;
}
int capwap_state_join(struct ap_dev *ap, CBuffer &buffer)
{
    CCapwapHeader *preq = NULL;
    string str = "";
    CBusiness business;

    uloop_timeout_cancel(&ap->cl->timeout);
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);

    preq = capwap_process_header(buffer, ap);
    if (preq == NULL)
    {
        ap->state = CAPWAP_STATE_QUIT;
        return CAPWAP_MESSAGE_DONE;
    }
    preq->Parse(buffer);
    preq->SaveTo(str);

    dlog(LOG_DEBUG, "%s", str.c_str());

    business.set_business_type(CAPWAP_BUSINESS_JOIN);
    business.set_business_string(str);

    if (BUSINESS_SUCCESS != business.Process(ap))
    {
        ap->state = CAPWAP_STATE_QUIT;
    }
    else
    {
        // TODO 确定是否存在上线升级配置，确认是否需要进入image_data状态
        ap->state = CAPWAP_STATE_CONFIGURE;
        uloop_timeout_set(&ap->cl->timeout, CAPWAP_JOIN_TIMEOUT);
    }

    SAFE_DELETE(preq);
    return CAPWAP_MESSAGE_DONE;
}
int capwap_state_image_data(struct ap_dev *ap, CBuffer &buffer)
{
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    ap->state ++;
    return CAPWAP_MESSAGE_DONE;
}
int capwap_state_configure(struct ap_dev *ap, CBuffer &buffer)
{
    CCapwapHeader *preq = NULL;
    string str = "";
    CBusiness business;

    uloop_timeout_cancel(&ap->cl->timeout);
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);

    preq = capwap_process_header(buffer, ap);
    if (preq == NULL)
    {
        ap->state = CAPWAP_STATE_QUIT;
        return CAPWAP_MESSAGE_DONE;
    }
    preq->Parse(buffer);
    preq->SaveTo(str);

    dlog(LOG_DEBUG, "%s", str.c_str());

    business.set_business_type(CAPWAP_BUSINESS_CONFIGURE);
    business.set_business_string(str);

    if (BUSINESS_SUCCESS != business.Process(ap))
    {
        ap->state = CAPWAP_STATE_QUIT;
    }
    else
    {
        ap->state = CAPWAP_STATE_DATA_CHECK;
        uloop_timeout_set(&ap->cl->timeout, CAPWAP_CONFIGURE_TIMEOUT);
    }

    SAFE_DELETE(preq);
    return CAPWAP_MESSAGE_DONE;

}
int capwap_state_data_check(struct ap_dev *ap, CBuffer &buffer)
{
    CCapwapHeader *preq = NULL;
    string str = "";
    CBusiness business;

    uloop_timeout_cancel(&ap->cl->timeout);
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);

    preq = capwap_process_header(buffer, ap);
    if (preq == NULL)
    {
        ap->state = CAPWAP_STATE_QUIT;
        return CAPWAP_MESSAGE_DONE;
    }
    preq->Parse(buffer);
    preq->SaveTo(str);

    dlog(LOG_DEBUG, "%s", str.c_str());

    business.set_business_type(CAPWAP_BUSINESS_DATA_CHECK);
    business.set_business_string(str);

    if (BUSINESS_SUCCESS != business.Process(ap))
    {
        ap->state = CAPWAP_STATE_QUIT;
    }
    else
    {
        ap->state = CAPWAP_STATE_RUN;
        uloop_timeout_set(&ap->cl->timeout, CAPWAP_DATA_CHECK_TIMEOUT);
    }

    SAFE_DELETE(preq);
    return CAPWAP_MESSAGE_DONE;
}
int capwap_state_run(struct ap_dev *ap, CBuffer &buffer)
{
    CCapwapHeader *preq = NULL;
    string str = "";
    CBusiness business;
    int timeout = CAPWAP_ECHO_TIMEOUT;

    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    preq = capwap_process_header(buffer, ap);
    if (preq == NULL)
    {
        return CAPWAP_MESSAGE_DONE;
    }
    preq->Parse(buffer);
    preq->SaveTo(str);

    dlog(LOG_DEBUG, "%s", str.c_str());

    switch(capwap_packet_type(preq))
    {
    case CAPWAP_PACKET_TYPE_DATA_TRANSFER_REQ:
        uloop_timeout_cancel(&ap->cl->timeout);
        business.set_business_type(CAPWAP_BUSINESS_DATA_TRANSFER);
        timeout = CAPWAP_DATA_TRANSFER_TIMEOUT;
        break;
    default:
        dlog(LOG_ERR, "%s.%d Unknown packet type %d", __FILE__, __LINE__, capwap_packet_type(preq));
        break;
    }
    business.set_business_string(str);

    if (BUSINESS_SUCCESS != business.Process(ap))
    {
        ap->state = CAPWAP_STATE_QUIT;
    }

    uloop_timeout_set(&ap->cl->timeout, timeout);
    SAFE_DELETE(preq);
    return CAPWAP_MESSAGE_DONE;
}
int capwap_state_reset(struct ap_dev *ap, CBuffer &buffer)
{
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    ap->state ++;
    return CAPWAP_MESSAGE_DONE;
}
int capwap_state_quit(struct ap_dev *ap, CBuffer &buffer)
{
    dlog(LOG_DEBUG, "ap state : %s", capwap_state_string[ap->state]);
    ap->state  = CAPWAP_STATE_QUIT;

    //struct client *cl = ap->cl;
    //cl->us->notify_state(cl->us);
    return CAPWAP_MESSAGE_DONE;
}