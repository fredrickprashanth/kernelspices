
#include <linux/tracepoint.h>
#include <trace/define_trace.h>

TRACE_EVENT(proc_tp_read,

        TP_PROTO(char *msg),

        TP_ARGS(msg),

        TP_STRUCT__entry(
                __string(msg,  msg)
        ),

        TP_fast_assign(
                __assign_str(__entry->msg, msg);
        ),

        TP_printk("msg=%s", __entry->msg)
);



TRACE_EVENT(proc_tp_write,

        TP_PROTO(char *msg),

        TP_ARGS(msg),

        TP_STRUCT__entry(
                __string(msg,  msg)
        ),

        TP_fast_assign(
                __assign_str(__entry->msg, msg);
        ),

        TP_printk("msg=%s", __entry->msg)
);
