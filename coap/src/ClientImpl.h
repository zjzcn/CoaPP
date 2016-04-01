/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#ifndef  __ClientImpl_h
#define  __ClientImpl_h

#include "IConnection.h"
#include "Logging.h"
#include "Message.h"
#include "NetUtils.h"
#include "Notifications.h"
#include "Responses.h"
#include "RestResponse.h"

#include <cassert>
#include <future>
#include <iostream>
#include <list>
#include <map>


namespace CoAP {

class Messaging;

class ClientImpl {
 public:
  ClientImpl(Messaging& messaging)
      : messaging_(messaging) {
  }

  ~ClientImpl();

  void onMessage(const Message& msg_received, in_addr_t fromIP, uint16_t fromPort);

  std::shared_ptr<Observable<CoAP::RestResponse>> GET(in_addr_t ip, uint16_t port, const std::string &uri, Type type);

  std::shared_ptr<Observable<CoAP::RestResponse>> PUT
      (in_addr_t ip, uint16_t port, const std::string &uri, const std::string &payload, Type type);

  std::shared_ptr<Observable<CoAP::RestResponse>> POST
      (in_addr_t ip, uint16_t port, const std::string &uri, const std::string &payload, Type type);

  std::shared_ptr<Observable<CoAP::RestResponse>> DELETE(in_addr_t ip, uint16_t port, const std::string &uri, Type type);

  std::shared_ptr<Observable<CoAP::RestResponse>> PING(in_addr_t ip, uint16_t port);

  std::shared_ptr<Observable<CoAP::RestResponse>> OBSERVE(in_addr_t ip, uint16_t port, const std::string &uri, Type type);

 private:

  uint64_t newToken() {
    // TODO: According to the RFC the token shall be randomized (at least 32-bit for internet traffic)
    // TODO: The length can be adapted based on the detection of attacks through reception of wrong tokens.
    return token_++;
  }

  /**
   * Sends a request, it sets up the mechanism to relate responses back to the request.
   * TODO: What to do, when nonconfirmable requests get lost and responses are never received.
   */
  std::shared_ptr<Observable<CoAP::RestResponse>> sendRequest(in_addr_t ip, uint16_t port, const Message &msg);

  uint16_t messageId_{0};

  uint64_t token_{0};

  std::mutex mutex_;

  std::map<uint64_t, std::weak_ptr<Observable<CoAP::RestResponse>>> notifications_;

  Messaging& messaging_;
};

}  // namespace CoAP;

#endif // __ClientImpl_h
