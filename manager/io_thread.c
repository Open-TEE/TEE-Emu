/*****************************************************************************
** Copyright (C) 2014 Intel Corporation.                                    **
**                                                                          **
** Licensed under the Apache License, Version 2.0 (the "License");          **
** you may not use this file except in compliance with the License.         **
** You may obtain a copy of the License at                                  **
**                                                                          **
**      http://www.apache.org/licenses/LICENSE-2.0                          **
**                                                                          **
** Unless required by applicable law or agreed to in writing, software      **
** distributed under the License is distributed on an "AS IS" BASIS,        **
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. **
** See the License for the specific language governing permissions and      **
** limitations under the License.                                           **
*****************************************************************************/

#include <errno.h>
#include <unistd.h>

#include "com_protocol.h"
#include "io_thread.h"
#include "manager_extern_resources.h"
#include "socket_help.h"
#include "tee_list.h"
#include "tee_logging.h"

/*!
 * \brief proc_fd_err
 * Process fd is erring
 * \param err_nro
 * \param fd
 * \param proc
 */
static void proc_fd_err(int err_nro, proc_t proc)
{
	/* Placeholder */

	err_nro = err_nro;
	proc = proc;
}

/*!
 * \brief io_fd_err
 * One of IO thread event fd is erring
 * \param err_nro
 * \param fd
 */
static void io_fd_err(int err_nro, int fd)
{
	/* Placeholder */

	err_nro = err_nro;
	fd = fd;
}

/*!
 * \brief check_event_fd_epoll_status
 * Checks event fd status and acts according to status. For example event_done_queue_fd.
 * \param event
 * \return
 */
static int check_event_fd_epoll_status(struct epoll_event *event)
{
	/* Placeholder */

	event = event;

	return 0;
}

static void send_msg(proc_t send_to, void *msg, int msg_len)
{
	int send_bytes, fd;

	if (msg_len == 0)
		return; /* Not an error */

	if (!msg || !send_to) {
		OT_LOG(LOG_ERR, "Sender proc or msg NULL")
		return;
	}

	if (send_to->p_type == proc_t_link)
		fd = send_to->content.sesLink.sockfd;
	else
		fd = send_to->content.process.sockfd;

	send_bytes = com_send_msg(fd, msg, msg_len);
	if (send_bytes != msg_len) {
		proc_fd_err(errno, send_to);
		return;
	}

	/* Special case: Open session message responses. Those should also send FD
	 * Question: Hide this dirty code from here to com_protocol? */
	if (com_get_msg_name(msg) == COM_MSG_NAME_OPEN_SESSION &&
		com_get_msg_type(msg) == COM_TYPE_RESPONSE) {

		if (send_fd(fd, ((struct com_msg_open_session *)msg)->sess_fd_to_caller) == -1) {
			OT_LOG(LOG_ERR, "Failed to send FD");
			proc_fd_err(errno, send_to);
		}
	}
}

void free_manager_msg(struct manager_msg *released_msg)
{
	free(released_msg->msg);
	free(released_msg);
}

void handle_done_queue(struct epoll_event *event)
{
	struct manager_msg *handled_msg;
	uint64_t done_event;

	if (check_event_fd_epoll_status(event))
		return; /* err msg logged */

	/* Reduce eventfd by one */
	if (read(event_done_queue_fd, &done_event, sizeof(uint64_t)) == -1) {
		OT_LOG(LOG_ERR, "Failed to reset eventfd");
		io_fd_err(errno, event_done_queue_fd);
	}

	/* Lock from logic thread */
	if (pthread_mutex_lock(&done_queue_mutex)) {
		OT_LOG(LOG_ERR, "Failed to lock the mutex");
		/* Lets hope that errot clear it shelf.. */
		return;
	}

	if (!list_is_empty(&done_queue.list)) {
		/* Queue is FIFO and therefore get just fist message */
		handled_msg = LIST_ENTRY(done_queue.list.next, struct manager_msg, list);
		list_unlink(&handled_msg->list);
	}

	if (pthread_mutex_unlock(&done_queue_mutex))
		OT_LOG(LOG_ERR, "Failed to unlock the mutex");

	send_msg(handled_msg->proc, handled_msg->msg, handled_msg->msg_len);

	free_manager_msg(handled_msg);
}
