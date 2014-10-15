// Copyright (C) 2014 sails Authors.
// All rights reserved.
//
// Filename: http.h
//
// Author: sailsxu <sailsxu@gmail.com>
// Created: 2014-10-11 14:31:46



#ifndef SAILS_NET_HTTP_H_
#define SAILS_NET_HTTP_H_

#include <stdio.h>
#include <map>
#include <string>

namespace sails {
namespace net {

///////////////////////////// message ///////////////////////////////////
#define MAX_HEADERS 13
#define MAX_PATH_SIZE 1024
#define MAX_ELEMENT_SIZE 2048


enum header_element { NONE = 0, FIELD, VALUE };

enum http_parser_type { HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH };


#undef TRUE
#define TRUE 1
#undef FALSE
#define FALSE 0

struct http_message {
  char *raw;
  enum http_parser_type type;
  int method;
  int status_code;
  char request_path[MAX_PATH_SIZE];
  char request_url[MAX_PATH_SIZE];
  char fragment[MAX_ELEMENT_SIZE];
  char query_string[MAX_ELEMENT_SIZE];
  char body[MAX_ELEMENT_SIZE];
  size_t body_size;
  char *host;
  char *userinfo;
  uint16_t port;
  int num_headers;
  enum header_element last_header_element;
  char headers[MAX_HEADERS][2][MAX_ELEMENT_SIZE];
  int should_keep_alive;

  char *upgrade;  // upgraded body

  int16_t http_major;
  int16_t http_minor;

  int message_begin_cb_called;
  int headers_complete_cb_called;
  int message_complete_cb_called;
  int message_complete_on_eof;
  int body_is_final;
};

void http_message_init(http_message *msg);
void delete_http_message(http_message *msg);


//////////////////////////// http request ////////////////////////////////
class HttpRequest {
 public:
  explicit HttpRequest(struct http_message *raw_data);

  ~HttpRequest();

  std::string getparam(std::string param_name);

  // new request for rpc client to send data
  void set_http_proto(int http_major, int http_minor);
  void set_request_method(int method);
  void set_default_header();
  int set_header(const char* key, const char *value);
  int set_body(const char* body);
  int to_str();
  char* get_raw();
 public:
  struct http_message *raw_data;
 private:
  std::map<std::string, std::string> param;
};



/////////////////////////// http response //////////////////////////////
class HttpResponse {
 public:
  HttpResponse();
  explicit HttpResponse(struct http_message *raw_data);
  ~HttpResponse();

  void set_http_proto(int http_major, int http_minor);
  void set_response_status(int response_status);
  int set_header(const char* key, const char *value);
  int set_body(const char* body);
  char *get_body();
  int to_str();
  char* get_raw();
 public:
  int connfd;
 private:
  struct http_message *raw_data;
  void set_default_header();
};

}  // namespace net
}  // namespace sails

#endif  // SAILS_NET _HTTP_H_
