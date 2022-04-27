//
// Created by Administrator on 2022/4/27.
//

#ifndef FM_HTTPSERVER_H
#define FM_HTTPSERVER_H

#include "TcpServer.h"
#include "InetAddress.h"
#include <memory>
#include <functional>
#include <sstream>
#include "spdlog/spdlog.h"

class RequestHeader{
public:
    void parse(const std::string& s)
    {
        // 寻找 \n\n
        std::string line;
        std::stringstream ss;

//        spdlog::info("find: {}", s.find("\r\n\r\n"));

        std::string  header = s.substr(0, s.find("\r\n\r\n"));
//        spdlog::info("s: {}", s);

        std::string data = s.substr(s.find("\r\n\r\n") + strlen("\r\n\r\n"), s.length() - s.find("\r\n\r\n") - 4);
//        spdlog::info("header: {}", header);
//        spdlog::info("data: {}", data);
        char a[256];
        char b[256];
        char c[256];
        ss << header;
        getline(ss, line, '\n');
        sscanf(line.c_str(), "%s %s %s", a, b, c);
        func_ = a;
        path_ = b;
        protocol_ = c;
        while (getline(ss, line, '\n')) {
            if(line.empty())break;
            sscanf(line.c_str(), "%s %s", a, b);
            std::string tmp = a;
            tmp.pop_back();
            header_[tmp] = b;
//            spdlog::info("{} : {}", tmp, b);

        }
        if (!data.empty())
        {
            data_ = data;
        }
    }

    std::string &getFunc(){
        return func_;
    }

    std::string &getPath(){
        return path_;
    }
    std::string &getProtocol(){
        return protocol_;
    }
    std::string &getData(){
        return data_;
    }
    std::map<std::string,std::string> &getHeader(){
        return header_;
    }

    std::string toString()
    {
        std::string s = func_ + " " + path_ + " " + protocol_ + '\n';
        for(auto& header:header_)
        {
            s += header.first + ": " + header.second  + '\n';
        }
        s+='\n';
        s += data_;
        return s;
    }


private:
    std::map<std::string,std::string> header_;
    std::string func_;
    std::string path_;
    std::string protocol_;
    std::string data_;
};


class ResponseHeader{
public:
    ResponseHeader()
    {
        header_["Content-Type"] = "text/html:charset=UTF-8";
        protocol_ = "HTTP/1.1";
        state_ = "200";
        stateName_ = "ok";

    }
    std::map<std::string,std::string> &getHeader(){
        return header_;
    }

    void setHeader(const std::string& name,const std::string& val)
    {
        header_[name] = val;
    }

    std::string toString()
    {
        std::stringstream ss;
        ss << strlen(data_.c_str());
        std::string len;
        ss >> len;
        header_["Content-Length"] = len;
        std::string s = protocol_ + " " + state_ + " " + stateName_ + '\n';
        for(auto& header:header_)
        {
            s += header.first + ": " + header.second  + '\n';
        }
        s+='\n';
        s += data_;
        return s;
    }

public:
    void setState(const std::string &state) {
        state_ = state;
    }

    void setStateName(const std::string &s) {
        stateName_ = s;
    }

    void setProtocol(const std::string &protocol) {
        protocol_ = protocol;
    }

    void setData(const std::string& data)
    {
        data_ = data;
    }


private:
    std::map<std::string,std::string> header_;
    std::string stateName_;
    std::string state_;
    std::string protocol_;
    std::string data_;
};


class HttpServer {
public:

    typedef std::function<void(RequestHeader &, ResponseHeader &)> HttpCallBack;

    void connectCallBack(const TcpConnection::ptr & conn)
    {
        if (conn->isConnected())
        {
            spdlog::info("http client connected");
        }else{
            spdlog::info("http client dis connected");
        }
    }

    void readCallBack(const TcpConnection::ptr& conn,Buffer& buffer , int len)
    {
        std::string request(buffer.begin() + buffer.getReadIndex(),len);


        RequestHeader requestHeader;
        ResponseHeader responseHeader;
        requestHeader.parse(request);

        if (httpCallBack)httpCallBack(requestHeader, responseHeader);

        conn->Send(responseHeader.toString());
        buffer.retrieve(len);
    }

    void errorCallBack(const TcpConnection::ptr& conn)
    {

        spdlog::info("http client error");

    }
    void closeCallBack(const TcpConnection::ptr& conn)
    {
        spdlog::info("http client close");
    }


    HttpServer(EventLoop* loop,InetAddress* address) : eventLoop(loop) , inetAddress(address) , tcpServer(eventLoop, inetAddress)
    {
        tcpServer.setConnectCallBack(std::bind(&HttpServer::connectCallBack, this , std::placeholders::_1));
        tcpServer.setReadCallBack(std::bind(&HttpServer::readCallBack, this , std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        tcpServer.setErrorCallBack(std::bind(&HttpServer::errorCallBack, this , std::placeholders::_1));
        tcpServer.setCloseCallBack(std::bind(&HttpServer::closeCallBack, this , std::placeholders::_1));
    }

    void start()
    {
        tcpServer.start();
    }

    void setHttpCallBack(const HttpCallBack& callBack)
    {
        httpCallBack = callBack;
    }

private:
    EventLoop* eventLoop;
    InetAddress *inetAddress;
    TcpServer tcpServer;
    HttpCallBack httpCallBack;
};


#endif //FM_HTTPSERVER_H
