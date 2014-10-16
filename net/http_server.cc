// Copyright (C) 2014 sails Authors.
// All rights reserved.
//
// Official git repository and contact information can be found at
// https://github.com/sails/sails and http://www.sailsxu.org/.
//
// Filename: http_server.cc
//
// Author: sailsxu <sailsxu@gmail.com>
// Created: 2014-10-15 16:22:41

#include "sails/net/http_server.h"

namespace sails {
namespace net {


HttpServer::HttpServer(uint32_t netThreadNum)
    :EpollServer<HttpRequest>(netThreadNum) {

}

HttpServer::~HttpServer() {
}

void HttpServer::Tdeleter(HttpRequest* data) {
  delete data;
}

void HttpServer::CreateConnectorCB(
    std::shared_ptr<net::Connector> connector) {
  // connector的data属性设置成parser,parser的data设置成解析的msg
  http_parser *parser = (http_parser*)malloc(sizeof(http_parser));
  http_parser_init(parser, HTTP_REQUEST);
  ParserFlag* parserFlag = new ParserFlag();
  parserFlag->message = (http_message*)malloc(sizeof(http_message));
  http_message_init(parserFlag->message);
  parser->data = parserFlag;
  
  connector->data.ptr = parser;
}

void HttpServer::ClosedConnectCB(
    std::shared_ptr<net::Connector> connector) {
  if (connector->data.ptr != NULL) {
    http_parser *parser = (http_parser*)connector->data.ptr;
    ParserFlag* parserFlag = (ParserFlag*)parser->data;
    delete parserFlag;
    delete parser;

    connector->data.ptr = NULL;
  }
}

HttpRequest* HttpServer::Parse(std::shared_ptr<net::Connector> connector) {
  HttpRequest* request = NULL;
  if (connector->data.ptr != NULL) {
    http_parser *parser = (http_parser*)connector->data.ptr;
    ParserFlag* flag = (ParserFlag*)parser->data;
    // 有数据解析,可能一次就已经解析出了多个数据包,所以即使connector中没有数据,依然会执行多次
    if (connector->readable() > 0) {
      char data[10000];
      memset(data, '\0', 10000);

      strncpy(data, connector->peek(), 10000);
      // 默认不是直接返回的,这儿通过把它这次解析过程中解析到的消息放到messagelist中
      http_parser *parser = (http_parser*)connector->data.ptr;
      size_t nparsed = sails_http_parser(parser,
                                         data,
                                         flag);
      if (nparsed > 0) {
        connector->retrieve(nparsed);
      }
    }
    if (flag->messageList.size() > 0) {
      struct http_message* message = flag->messageList.front();
      flag->messageList.pop_front();
      request = new HttpRequest(message);
    }
  }
  return request;
}



}  // namespace net          
}  // namespace sails