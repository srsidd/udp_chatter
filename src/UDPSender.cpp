#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <thread>

using boost::asio::ip::address;
using boost::asio::ip::udp;

std::string send_to_ip = "127.0.0.1";
int send_to_port = 10001;


void udpMessageSender(std::string& in)
{
    boost::asio::io_service io_service;
    udp::socket socket( io_service );
    // std::cout << "ip: " << send_to_ip << " port: " << send_to_port << std::endl;
    udp::endpoint remote_endpoint = udp::endpoint(address::from_string(send_to_ip), send_to_port);
    socket.open(udp::v4());

    boost::system::error_code err;
    auto sent = socket.send_to(boost::asio::buffer(in), remote_endpoint, 0, err);
    socket.close();
}

std::vector<boost::program_options::option> endOfOptsParser(std::vector<std::string>& args)
{
    std::vector<boost::program_options::option> result;

    std::vector<std::string>::const_iterator i(args.begin());
    if (i != args.end() && *i == "--")
    {
        for (++i; i != args.end(); ++i)
        {
            boost::program_options::option opt;
            opt.string_key = "msg";
            opt.value.push_back(*i);      //  <== here
            opt.original_tokens.push_back(*i);
            result.push_back(opt);
        }
    args.clear();
  }
  return result;
}

int main( int argc, char *argv[] )
{
    boost::asio::io_service io_service;

    boost::program_options::options_description description("Command line tool for sending udp messages, usage: ./udp_sender [args...] -- msg");

    description.add_options()
    ("help,h",    "Display this help message and exit")
    ("version,v", "Display the version number")
    ("rate,r",     boost::program_options::value<double>(), "Data publish rate in hz")
    ("ip_addr,i",  boost::program_options::value<std::string>()->default_value("127.0.0.1"), "IP address to publish messages to")
    ("port,p",     boost::program_options::value<int>()->default_value(10001), "Port to publish messages to")
    ("once,1",     boost::program_options::value<std::string>()->implicit_value("stdin"), "Publish one message and exit")
    ("msg",        boost::program_options::value<std::vector<std::string>>()->multitoken(), "Message to be published");

    boost::program_options::variables_map vm;

    boost::program_options::command_line_parser clparser(argc, argv);
    clparser.extra_style_parser(endOfOptsParser);

    boost::program_options::store(clparser.options(description).run(), vm);
    boost::program_options::notify(vm);

    // Set number of decimal place for cout to 3
    std::cout << std::setprecision(3) << std::fixed;

    double rate = 0.0;
    bool pub_once = false;

    if (vm.count("help"))
    {
        std::cout << description << std::endl;
    }

    if (vm.count("rate"))
    {
        rate = vm["rate"].as<double>();
    }

    if (vm.count("once"))
    {
         pub_once = true;
    }

    if (argc < 2) {
        std::cout << description << std::endl;
        return 0;
    }

    if (vm.count("ip_addr"))
    {
        send_to_ip = vm["ip_addr"].as<std::string>();
    }
    if (vm.count("port"))
    {
        send_to_port = vm["port"].as<int>();
    }

    std::vector<std::string> const& udp_string = vm["msg"].as<std::vector<std::string>>();
    std::ostringstream udp_stream;

    // std::string udp_msg = std::accumulate(udp_string.begin(), udp_string.end(), std::string(" "));
    std::copy(udp_string.begin(), udp_string.end(), std::ostream_iterator<std::string>(udp_stream, " "));
    std::string udp_msg = udp_stream.str();

    if (pub_once)
    {
        std::cout << "Publishing message: " << udp_msg << std::endl;
        udpMessageSender(udp_msg);
    }
    else if (rate == 0)
    {
        std::cout << "Publish rate not set, publishing message only once: " << udp_msg << std::endl;
        udpMessageSender(udp_msg);
    }
    else
    {
        std::cout << "Publish rate set to " << rate << " hz" << std::endl;
        while(true)
        {
            std::cout << "Publishing message: " << udp_msg << std::endl;
            udpMessageSender(udp_msg);
            std::this_thread::sleep_for( std::chrono::duration<double>( (double)1 / rate ) );
        }
    }

    io_service.stop();
    return 0;
}
