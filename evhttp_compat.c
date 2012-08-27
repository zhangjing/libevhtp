#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <evhtp.h>
#include "evhttp_compat.h"

struct evhttp *
evhttp_new(struct event_base * base) {
    return evhtp_new(base, NULL);
}

int
evhttp_bind_socket(struct evhttp * http, const char * address, ev_uint16_t port) {
    return evhtp_bind_socket(http, address, port, 1024);
}

void
evhttp_set_max_body_size(struct evhttp * http, ev_ssize_t max_body_size) {
    return evhtp_set_max_body_size(http, (uint64_t)max_body_size);
}

int
evhttp_set_cb(struct evhttp * http, const char * path,
              void (* cb)(struct evhttp_request *, void *), void * cb_arg) {
    if (evhtp_set_cb(http, path, cb, arg) == NULL) {
        return 0;
    }

    return -1;
}

void
evhttp_set_gencb(struct evhttp * http,
                 void (* cb)(struct evhttp_request *, void *), void * arg) {
    return evhtp_set_gencb(http, cb, arg);
}

int
evhttp_add_virtual_host(struct evhttp * http, const char * pattern, struct evhttp * vhost) {
    return evhtp_add_vhost(http, pattern, vhost);
}

/* TODO: evhttp_remove_virtual_host */

int
evhttp_add_server_alias(struct evhttp * http, const char * alias) {
    return evhtp_add_alias(http, alias);
}

/* TODO: evhttp_remove_server_alias */

void
evhttp_set_timeout(struct evhttp * http, int timeout_in_secs) {
    struct timeval tv;

    tv.tv_sec  = timeout_in_secs;
    tv.tv_usec = 0;

    return evhtp_set_timeouts(http, &tv, NULL);
}

/* TODO: evhttp_send_error, will just be a frontend to send_reply */

void
evhttp_send_reply(struct evhttp_request * req, int code, const char * reason,
                  struct evbuffer * databuf) {
    /* TODO, need to add a reason thingy */
    evbuffer_add_buffer(req->buffer_out, databuf);

    return evhtp_send_reply(req, code);
}

void
evhttp_send_reply_start(struct evhttp_request * req, int code, const char * reason) {
    return evhtp_send_reply_chunk_start(req, code);
}

void
evhttp_send_reply_chunk(struct evhttp_request * req, struct evbuffer * databuf) {
    return evhtp_send_reply_chunk(req, databuf);
}

void
evhttp_send_reply_end(struct evhttp_request * req) {
    return evhtp_send_reply_end(req);
}

/*
 * TODO evhttp_request_new, it exists but not exported
 * TODO: evhttp_request_set_chunked_cb, will have to create an on_chunk_read hook
 * TODO: evhttp_request_free, it exists but not exported
 */

struct bufferevent *
evhttp_connection_get_bufferevent(struct evhttp_connection * evconn) {
    return evhtp_connection_get_bev(evconn);
}

/* TODO evhtp_request_own(), must create function like take_ownership() but
 * without freeing underlying structures */

int
evhttp_request_is_owned(struct evhttp_request * req) {
    return (int)req->owner;
}

struct evhttp_connection *
evhttp_request_get_connection(struct evhttp_request * req) {
    return evhtp_request_get_connection(req);
}

struct event_base *
evhttp_connection_get_base(struct evhttp_connection * conn) {
    return conn->evbase;
}

void
evhttp_connection_free(struct evhttp_connection * evcon) {
    return evhtp_connection_free(evcon);
}

/*
 * TODO set_local_address
 * TODO set_local_port
 */

void
evhttp_connection_set_timeout(struct evhttp_connection * evconn, int timeout_in_secs) {
    struct tv tv;

    tv.tv_sec  = timeout_in_secs;
    tv.tv_usec = 0;

    return evhtp_connection_set_timeouts(evconn, &tv, NULL);
}

/*
 * TODO connection_set_retries
 * TODO connection_set_closecb (should use on_error hook
 * TODO connection_get_peer
 * TODO make_request
 * TODO cancel_request
 */
const char *
evhttp_request_get_uri(const struct evhttp_request * req) {
    if (!req->uri) {
        return NULL;
    }

    return req->uri->query_raw;
}

const
evhttp_uri *
evhttp_request_get_evhttp_uri(struct evhttp_request * req) {
    return req->uri;
}

