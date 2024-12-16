#ifndef __PROTO_SERVER_H
#define __PROTO_SERVER_H

extern "C"
{
    #include <event2/event.h>
    #include <event2/buffer.h>
    #include <event2/bufferevent.h>
}

#include <string>
#include <map>
#include <functional>
#include <mutex>

#include "proto_utils.h"

namespace proto
{
    class server;

    class connection
    {
    public:
        connection();
        virtual ~connection();

        virtual void setup(evutil_socket_t fd, struct bufferevent* bev, server* srv);
        void send(const void* data, size_t numBytes);
        virtual void on_read_handler(const void* buffer, size_t numBytes);

        struct bufferevent* mBufferEvent;
        evutil_socket_t mFd;
        server* mServer;
    };


    class server
    {
    friend class connection;
    public:
        // Callbacks
        typedef std::function<void(server*, connection*)>   on_connected;
        typedef std::function<void(server*, connection*)>   on_disconnected;
        typedef std::function<connection*(server*)>         on_accepted;
        typedef std::function<void(server*, connection*, const void* buffer, size_t num)> on_received;

        server();
        virtual ~server();

        void set_callbacks(on_connected client_conn, on_disconnected client_disconn,
                           on_accepted client_accept, on_received client_recv);
        void setup(const unsigned short& port) ;
        void update();
        void sendToAllClients(const char* data, size_t len);
        void addConnection(evutil_socket_t fd, connection* connection);
        void removeConnection(evutil_socket_t fd);

    protected:
        static void listenerCallback(
             struct evconnlistener* listener
            ,evutil_socket_t socket
            ,struct sockaddr* saddr
            ,int socklen
            ,void* server
        );

        static void signalCallback(evutil_socket_t sig, short events, void* server);
        static void writeCallback(struct bufferevent*, void* server);
        static void readCallback(struct bufferevent*, void* connection);
        static void eventCallback(struct bufferevent*, short, void* server);

        struct sockaddr_in sin;
        struct event_base* base;
        struct event* signal_event;
        struct evconnlistener* listener;

        std::map<evutil_socket_t, connection*> connections;
        on_connected client_conn;
        on_disconnected client_disconn;
        on_accepted client_accept;
        on_received client_recv;
        std::recursive_mutex mConnectionsMutex;

        virtual connection* on_create_conn();
    };


    class msgconnection: public connection
    {
    public:
        msgconnection();
        ~msgconnection();
        void setup(evutil_socket_t fd, struct bufferevent* bev, server* srv) override;

        // Send length prefixed message
        void send_msg(const void* buffer, size_t num);

    protected:
        msgparser mParser;

        // Uses msgparser to handle incoming data
        void on_read_handler(const void* buffer, size_t numBytes) override;
    };

    class msgserver: public server
    {
    friend class msgconnection;
    public:
        typedef std::function<void(msgserver&, msgconnection& conn, const void* buffer, size_t num)> on_message;

        msgserver();
        ~msgserver();

        void set_msg_callback(on_message cb);
        void send_msg(connection& conn, const void* data, size_t num);
        void send_msg(const void* data, size_t num);
    protected:
        on_message mMessageCb;
        connection* on_create_conn() override;
    };
} // end of namespace

#endif
