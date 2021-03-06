/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MOS_LIBS_RPC_COMMON_SRC_MG_RPC_MG_RPC_CHANNEL_H_
#define CS_MOS_LIBS_RPC_COMMON_SRC_MG_RPC_MG_RPC_CHANNEL_H_

#include <inttypes.h>
#include <stdbool.h>

#include "common/mg_str.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mg_rpc_authn;

enum mg_rpc_channel_event {
  MG_RPC_CHANNEL_OPEN,
  MG_RPC_CHANNEL_FRAME_RECD,
  MG_RPC_CHANNEL_FRAME_RECD_PARSED,
  MG_RPC_CHANNEL_FRAME_SENT,
  MG_RPC_CHANNEL_CLOSED,
};

struct mg_rpc_channel {
  void (*ev_handler)(struct mg_rpc_channel *ch, enum mg_rpc_channel_event ev,
                     void *ev_data);

  void (*ch_connect)(struct mg_rpc_channel *ch);

  bool (*send_frame)(struct mg_rpc_channel *ch, const struct mg_str f);

  /*
   * Close tells the channel to wind down.
   * This applies to persistent channels as well: if a channel is told to close,
   * it means the system wants it gone, for real.
   * MG_RPC_CHANNEL_CLOSED should be emitted when channel closes.
   */
  void (*ch_close)(struct mg_rpc_channel *ch);

  /*
   * Destroy is invoked in response to the MG_RPC_CHANNEL_CLOSED after all
   * cleanup is finished. The channel should dispose of the channel struct
   * as well as the channel struct itself.
   * ch_destroy is guaranteed to be the last thing to happen to a channel.
   */
  void (*ch_destroy)(struct mg_rpc_channel *ch);

  const char *(*get_type)(struct mg_rpc_channel *ch);

  bool (*is_persistent)(struct mg_rpc_channel *ch);

  /* Return free form information about the peer. Caller must free() it. */
  char *(*get_info)(struct mg_rpc_channel *ch);

  /*
   * Get authentication info, if present, from the channel and populate it into
   * the given authn struct. Returns true if the authn info is present; false
   * otherwise. Caller should call mg_rpc_authn_free() on it afterwards.
   */
  bool (*get_authn_info)(struct mg_rpc_channel *ch, struct mg_rpc_authn *authn);

  void *channel_data;
  void *mg_rpc_data;
  void *user_data;
};

#ifdef __cplusplus
}
#endif

#endif /* CS_MOS_LIBS_RPC_COMMON_SRC_MG_RPC_MG_RPC_CHANNEL_H_ */
