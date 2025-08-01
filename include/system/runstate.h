#ifndef SYSTEM_RUNSTATE_H
#define SYSTEM_RUNSTATE_H

#include "qapi/qapi-types-run-state.h"
#include "qemu/notify.h"

bool runstate_check(RunState state);
void runstate_set(RunState new_state);
RunState runstate_get(void);
bool runstate_is_running(void);
bool runstate_needs_reset(void);
void runstate_replay_enable(void);

typedef void VMChangeStateHandler(void *opaque, bool running, RunState state);
typedef int VMChangeStateHandlerWithRet(void *opaque, bool running, RunState state);

/**
 * qemu_add_vm_change_state_handler:
 * @cb: the callback to invoke
 * @opaque: user data passed to the callback
 *
 * Register a callback function that is invoked when the vm starts or stops
 * running.
 *
 * Returns: an entry to be freed using qemu_del_vm_change_state_handler()
 */
VMChangeStateEntry *qemu_add_vm_change_state_handler(VMChangeStateHandler *cb,
                                                     void *opaque);
/**
 * qemu_add_vm_change_state_handler_prio:
 * @cb: the callback to invoke
 * @opaque: user data passed to the callback
 * @priority: low priorities execute first when the vm runs and the reverse is
 *            true when the vm stops
 *
 * Register a callback function that is invoked when the vm starts or stops
 * running.
 *
 * Returns: an entry to be freed using qemu_del_vm_change_state_handler()
 */
VMChangeStateEntry *qemu_add_vm_change_state_handler_prio(
        VMChangeStateHandler *cb, void *opaque, int priority);
VMChangeStateEntry *
/**
 * qemu_add_vm_change_state_handler_prio_full:
 * @cb: the main callback to invoke
 * @prepare_cb: a callback to invoke before the main callback
 * @cb_ret: the main callback to invoke with return value
 * @opaque: user data passed to the callbacks
 * @priority: low priorities execute first when the vm runs and the reverse is
 *            true when the vm stops
 *
 * Register a main callback function and an optional prepare callback function
 * that are invoked when the vm starts or stops running. The main callback and
 * the prepare callback are called in two separate phases: First all prepare
 * callbacks are called and only then all main callbacks are called. As its
 * name suggests, the prepare callback can be used to do some preparatory work
 * before invoking the main callback.
 *
 * Returns: an entry to be freed using qemu_del_vm_change_state_handler()
 */
qemu_add_vm_change_state_handler_prio_full(VMChangeStateHandler *cb,
                                           VMChangeStateHandler *prepare_cb,
                                           VMChangeStateHandlerWithRet *cb_ret,
                                           void *opaque, int priority);
VMChangeStateEntry *qdev_add_vm_change_state_handler(DeviceState *dev,
                                                     VMChangeStateHandler *cb,
                                                     VMChangeStateHandlerWithRet *cb_ret,
                                                     void *opaque);
VMChangeStateEntry *qdev_add_vm_change_state_handler_full(
    DeviceState *dev, VMChangeStateHandler *cb, VMChangeStateHandler *prepare_cb,
    VMChangeStateHandlerWithRet *cb_ret, void *opaque);
void qemu_del_vm_change_state_handler(VMChangeStateEntry *e);
/**
 * vm_state_notify: Notify the state of the VM
 *
 * @running: whether the VM is running or not.
 * @state: the #RunState of the VM.
 *
 * Return the result of the callback which has return value.
 * If no callback has return value, still return 0 and the
 * upper layer should not do additional processing.
 */
int vm_state_notify(bool running, RunState state);

static inline bool shutdown_caused_by_guest(ShutdownCause cause)
{
    return cause >= SHUTDOWN_CAUSE_GUEST_SHUTDOWN;
}

/*
 * In a "live" state, the vcpu clock is ticking, and the runstate notifiers
 * think we are running.
 */
static inline bool runstate_is_live(RunState state)
{
    return state == RUN_STATE_RUNNING || state == RUN_STATE_SUSPENDED;
}

void vm_start(void);

/**
 * vm_prepare_start: Prepare for starting/resuming the VM
 *
 * @step_pending: whether any of the CPUs is about to be single-stepped by gdb
 */
int vm_prepare_start(bool step_pending);

/**
 * vm_resume: If @state is a live state, start the vm and set the state,
 * else just set the state.
 *
 * @state: the state to restore
 */
void vm_resume(RunState state);

int vm_stop(RunState state);
int vm_stop_force_state(RunState state);
int vm_shutdown(void);
void vm_set_suspended(bool suspended);
bool vm_get_suspended(void);

typedef enum WakeupReason {
    /* Always keep QEMU_WAKEUP_REASON_NONE = 0 */
    QEMU_WAKEUP_REASON_NONE = 0,
    QEMU_WAKEUP_REASON_RTC,
    QEMU_WAKEUP_REASON_PMTIMER,
    QEMU_WAKEUP_REASON_OTHER,
} WakeupReason;

void qemu_system_reset_request(ShutdownCause reason);
void qemu_system_suspend_request(void);
void qemu_register_suspend_notifier(Notifier *notifier);
bool qemu_wakeup_suspend_enabled(void);
void qemu_system_wakeup_request(WakeupReason reason, Error **errp);
void qemu_system_wakeup_enable(WakeupReason reason, bool enabled);
void qemu_register_wakeup_notifier(Notifier *notifier);
void qemu_register_wakeup_support(void);
void qemu_system_shutdown_request_with_code(ShutdownCause reason,
                                            int exit_code);
void qemu_system_shutdown_request(ShutdownCause reason);
void qemu_system_powerdown_request(void);
void qemu_register_powerdown_notifier(Notifier *notifier);
void qemu_register_shutdown_notifier(Notifier *notifier);
void qemu_system_debug_request(void);
void qemu_system_vmstop_request(RunState reason);
void qemu_system_vmstop_request_prepare(void);
bool qemu_vmstop_requested(RunState *r);
ShutdownCause qemu_shutdown_requested_get(void);
bool qemu_force_shutdown_requested(void);
ShutdownCause qemu_reset_requested_get(void);
void qemu_system_killed(int signal, pid_t pid);
void qemu_system_reset(ShutdownCause reason);
void qemu_system_guest_panicked(GuestPanicInformation *info);
void qemu_system_guest_crashloaded(GuestPanicInformation *info);
void qemu_system_guest_pvshutdown(void);
bool qemu_system_dump_in_progress(void);

#endif

