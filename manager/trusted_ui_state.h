/*****************************************************************************
** Copyright (C) 2015 Intel Corporation.                                    **
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

#ifndef __TRUSTED_UI_STATE_H__
#define __TRUSTED_UI_STATE_H__

/* TODO: Move proc_t definition from extern_resources to its own file */
/* define an opaque structure to handle the process related information */
typedef struct __proc *proc_t;

/*
 * TUI state machine
 *
 * disconnected -> connected -> session on going
 *        ^-__________/_____________/
 */
enum trusted_ui_connection_state {
	TUI_DISCONNECTED = 0,
	TUI_CONNECTED,
	TUI_SESSION
};

struct trusted_ui_state {
	proc_t proc;
	enum trusted_ui_connection_state state;

        /* Table to hold TUI requests from TAs */
        HASHTABLE requests;

};

#endif /* __TRUSTED_UI_STATE_H__ */
