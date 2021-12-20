#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>

#define SENDER_IPADDRESS "127.0.0.1" // "192.168.1.64"
#define SENDER_UDP_PORT 10001

#define LISTENER_UDP_PORT 35768

using boost::asio::ip::address;
using boost::asio::ip::udp;

void Sender( std::string in )
{
    boost::asio::io_service io_service;
    udp::socket socket( io_service );
    udp::endpoint remote_endpoint = udp::endpoint( address::from_string( SENDER_IPADDRESS ), SENDER_UDP_PORT );
    socket.open( udp::v4() );

    boost::system::error_code err;
    auto sent = socket.send_to( boost::asio::buffer( in ), remote_endpoint, 0, err );
    socket.close();
    std::cout << "Sent Payload - " << sent << "\n";
}

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time( 0 );
    return ctime( &now );
}

class UDPListener
{
public:
    UDPListener( boost::asio::io_service &io_service ) : socket_( io_service, udp::endpoint( udp::v4(), LISTENER_UDP_PORT ) )
    {
        StartReceive();
    }

private:
    void StartReceive()
    {
        socket_.async_receive_from( boost::asio::buffer( recv_buffer_ ), remote_endpoint_,
                                    boost::bind( &UDPListener::HandleReceive, this, boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred ) );
    }

    void HandleReceive( const boost::system::error_code &error, std::size_t bytes_transferred )
    {
        if( !error || error == boost::asio::error::message_size )
        {
            boost::shared_ptr< std::string > message( new std::string( make_daytime_string() ) );

            std::cout << "Received: '" << std::string(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred) << "' (" << error.message() << ")\n";

            StartReceive();
        }
        else
        {
            std::cout << __PRETTY_FUNCTION__ << " Error in received message, boost error code: " << error.message()  << std::endl;
        }
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array< char, 1024 > recv_buffer_;
};

int main( int argc, char *argv[] )
{

    boost::asio::io_service io_service;
    UDPListener listener( io_service );
    std::thread r( [&] { io_service.run(); } );

    std::string input = argc > 1 ? argv[1] : "hello world";

    int i = 0;
    while( true )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
        if (++i % 10 == 0)
            Sender( "Hello world" );
    }

    io_service.stop();
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

    if( r.joinable() )
    {
        r.join();
    }

    std::cout << "\nUDP chatter killed!!!\n";
}
