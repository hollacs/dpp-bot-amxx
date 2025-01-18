#pragma once

#include "amxxmodule.h"

cell AMX_NATIVE_CALL native_is_made(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_is_running(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_make(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_destroy(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_start(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_shutdown(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_register_cmd(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_create_message(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_msg_reply(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_cmd_reply(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_msg(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_msg_set_channel(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_msg_set_flags(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_msg_add_embed(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_embed(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_embed_set_title(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_embed_set_desc(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_embed_set_color(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_set_presence(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_escape_markdown(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_is_msg_author_bot(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_get_msg_author_username(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_get_msg_content(AMX* amx, cell* params);
cell AMX_NATIVE_CALL native_get_msg_channel_id(AMX* amx, cell* params);

cell AMX_NATIVE_CALL native_get_cmd_channel_id(AMX* amx, cell* params);