// +--------------------------------- DISCLAIMER --------------------------------+
// |                                                                             |
// |                                                                             |
// | This code is provided for educational and non-commercial purposes only.     |
// | You are authorized to modify and distribute this code, but it comes with    |
// | absolutely no warranty, implied or otherwise. The authors shall not be      |
// | held liable for any damages or issues arising from the use of this code.    |
// | Use at your own risk.                                                       |
// |                                                                             |
// |                                                                             |
// +-----------------------------------------------------------------------------+

// This code is designed for reading and writing network parameters on a strobe controller (IPSC or HPSC).
// To use this code, update the IP addresses of both the computer and the strobe controller.
// Then, you can write the desired command and utilize the SendPacket function to send it.


#include <iostream>
#include <cstring>
#include <uv.h>

uv_udp_t g_sendSocket;
uv_loop_t* g_loop;

// HPSC Commands
// Set static IP 
unsigned char requestPacketHPSC[] = { 0x01, 0x27, 0x6C, 0xD1, 0x46, 0x10, 0x01, 0x26, 0x1F, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0xBF, 0x04 };

// New IP:192.168.1.26
unsigned char requestPacketHPSC1[] = { 0x01, 0x27, 0x6C, 0xD1, 0x46, 0x10, 0x01, 0x26, 0x1F, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x10, 0x01, 0x1A, 0x0A, 0x83, 0x04 };


// IPSC Commands
unsigned char requestPacketIPSC[] = { "Smartek#I#6CD1XXXXXXXX#F#192.168.1.30#255.255.255.0#IPSC1\r" };

// Send callback function
void SendCb(uv_udp_send_t* req, int status) {
    if (status == -1) {
        fprintf(stderr, "Send error!\n");
        return;
    }
    std::cout << "Request sent successfully" << std::endl;
    free(req);
}

void SendPacket(uv_udp_t* socket, const unsigned char* packet, size_t packetSize, const sockaddr_in* addr) {
    uv_udp_send_t* send_req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
    uv_buf_t buffer = uv_buf_init((char*)packet, packetSize);
    int status = uv_udp_send(send_req, socket, &buffer, 1, (const struct sockaddr*)addr, SendCb);

    if (status < 0) {
        std::cerr << "Failed to send UDP packet: " << uv_strerror(status) << std::endl;
    }
    else {
        std::cout << "UDP packet queued for sending." << std::endl;
    }
}


void AllocCb(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void CloseUDPCb(uv_handle_t* handle) {
    fprintf(stderr, "UDP Handle closed!\n");
}

int main() {
    g_loop = uv_default_loop();
    int status;

    uv_udp_init(g_loop, &g_sendSocket);
    struct sockaddr_in local_addr;
    uv_ip4_addr("172.16.101.6", 54188, &local_addr);    // Change for the IP address of your computer
    uv_udp_bind(&g_sendSocket, (const struct sockaddr*)&local_addr, 0);

    uv_udp_set_broadcast(&g_sendSocket, 1);

    struct sockaddr_in send_addr_HPSC;
    uv_ip4_addr("255.255.255.255", 30311, &send_addr_HPSC); // Try first with this IP, if fail change for the IP address of the Strobe

    // Send HPSC Commands
    //SendPacket(&g_sendSocket, requestPacketHPSC, sizeof(requestPacketHPSC), &send_addr_HPSC);
    //SendPacket(&g_sendSocket, requestPacketHPSC1, sizeof(requestPacketHPSC1), &send_addr_HPSC);

    // Send IPSC Commands
    SendPacket(&g_sendSocket, requestPacketIPSC, sizeof(requestPacketIPSC), &send_addr_HPSC);

    uv_run(g_loop, UV_RUN_DEFAULT);

    uv_udp_recv_stop(&g_sendSocket);
    uv_close((uv_handle_t*)&g_sendSocket, nullptr);
    uv_loop_close(g_loop);

    return 0;
}
