//
// Created by Administrator on 2022/4/27.
//

#include "HttpServer.h"

void RequestHeader::parse(const std::string &s)     {
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

std::string &RequestHeader::getFunc() {
    return func_;
}

std::string &RequestHeader::getPath() {
    return path_;
}

std::string &RequestHeader::getProtocol(){
    return protocol_;
}

std::string &RequestHeader::getData() {
    return data_;
}

std::map<std::string, std::string> &RequestHeader::getHeader() {
    return header_;
}

std::string RequestHeader::toString()     {
    std::string s = func_ + " " + path_ + " " + protocol_ + '\n';
    for(auto& header:header_)
    {
        s += header.first + ": " + header.second  + '\n';
    }
    s+='\n';
    s += data_;
    return s;
}

std::map<std::string, std::string> &RequestHeader::getParam()    {
    std::stringstream ss;
    unsigned long pos = 0;
    std::string p;
    if (func_ == "GET")
    {
        p = path_;
        pos = p.find("/?");
        if (pos != std::string::npos) {
            std::string s = p.substr(pos + 2, p.length() - pos - 2);
            ss << s;
        }
    }else{
        p = data_;
        ss << p;
    }

    std::string param;
    while (getline(ss, param, '&')) {
        if(!param.empty())
        {
            auto pos2 = param.find('=');

            param_[param.substr(0, pos2)] = param.substr(pos2 + 1, param.length() - pos2 - 1);
//                spdlog::info(param.substr(0, pos2));
//                spdlog::info(param.substr(pos2 + 1, param.length() - pos2 - 1));
        }
    }

    return param_;
}

ResponseHeader::ResponseHeader()    {
    header_["Content-Type"] = "text/html:charset=UTF-8";
    protocol_ = "HTTP/1.1";
    state_ = "200";
    stateName_ = "ok";

}

std::map<std::string, std::string> &ResponseHeader::getHeader(){
    return header_;
}

void ResponseHeader::setHeader(const std::string &name, const std::string &val)  {
    header_[name] = val;
}

std::string ResponseHeader::toString()     {
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

void ResponseHeader::setState(const std::string &state) {
    state_ = state;
}

void ResponseHeader::setStateName(const std::string &s) {
    stateName_ = s;
}

void ResponseHeader::setProtocol(const std::string &protocol){
    protocol_ = protocol;
}

void ResponseHeader::setData(const std::string &data)    {
    data_ = data;
}

void HttpServer::connectCallBack(const TcpConnection::ptr &conn)     {
    if (conn->isConnected())
    {
        spdlog::info("http client connected");
    }else{
        spdlog::info("http client dis connected");
    }
}

void HttpServer::readCallBack(const TcpConnection::ptr &conn, Buffer &buffer, int len)     {
    std::string request(buffer.begin() + buffer.getReadIndex(),len);


    RequestHeader requestHeader;
    ResponseHeader responseHeader;
    requestHeader.parse(request);

    if (httpCallBack)httpCallBack(requestHeader, responseHeader);

    conn->Send(responseHeader.toString());
    buffer.retrieve(len);
}

void HttpServer::errorCallBack(const TcpConnection::ptr &conn)    {

    spdlog::info("http client error");

}

void HttpServer::closeCallBack(const TcpConnection::ptr &conn)     {
    spdlog::info("http client close");
}

HttpServer::HttpServer(EventLoop *loop, InetAddress *address)   : eventLoop(loop) , inetAddress(address) , tcpServer(eventLoop, inetAddress)  {
    tcpServer.setConnectCallBack(std::bind(&HttpServer::connectCallBack, this , std::placeholders::_1));
    tcpServer.setReadCallBack(std::bind(&HttpServer::readCallBack, this , std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    tcpServer.setErrorCallBack(std::bind(&HttpServer::errorCallBack, this , std::placeholders::_1));
    tcpServer.setCloseCallBack(std::bind(&HttpServer::closeCallBack, this , std::placeholders::_1));
}

void HttpServer::start()     {
    tcpServer.start();
}

void HttpServer::setHttpCallBack(const HttpServer::HttpCallBack &callBack)    {
    httpCallBack = callBack;
}
