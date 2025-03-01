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

// This code is designed for reading and writing user parameters on a strobe controller (IPSC or HPSC).
// To use this code, simply update the IP address with the appropriate IP address of your strobe controller.
// Then, you can write the desired command and use the functions sendPacketIPSC or sendPacketHPSC to send it.


#include <iostream>
#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib")

    void sendPacketHPSC(SOCKET sockfd, const unsigned char* packet, int packetSize) {
        int bytesSent = 0;

        while (bytesSent < packetSize) {
            int sent = send(sockfd, reinterpret_cast<const char*>(packet + bytesSent), packetSize - bytesSent, 0);
            if (sent == SOCKET_ERROR) {
                std::cerr << "Error sending request: " << WSAGetLastError() << std::endl;
                closesocket(sockfd);
                WSACleanup();
                exit(1);
            }
            bytesSent += sent;
        }

        std::cout << "Request packet: ";
        for (int i = 0; i < packetSize; ++i) {
            printf("%02X ", packet[i]);
        }
        std::cout << std::endl;

        std::cout << "Request sent successfully" << std::endl;

        char responseBuffer[50];
        int bytesReceived = recv(sockfd, responseBuffer, sizeof(responseBuffer), 0);
        std::cout << "Received response: ";
        for (int i = 0; i < bytesReceived; ++i) {
            printf("%02X ", static_cast<unsigned char>(responseBuffer[i]));
        }
        std::cout << std::endl;
    }

    void sendPacketIPSC(SOCKET sockfd, const unsigned char* packet, int packetSize) {
        int bytesSent = 0;

        while (bytesSent < packetSize) {
            int sent = send(sockfd, reinterpret_cast<const char*>(packet + bytesSent), packetSize - bytesSent, 0);
            if (sent == SOCKET_ERROR) {
                std::cerr << "Error sending request: " << WSAGetLastError() << std::endl;
                closesocket(sockfd);
                WSACleanup();
                exit(1);
            }
            bytesSent += sent;
        }

        std::cout << "Request sent successfully" << std::endl;

        char responseBuffer[250];
        int bytesReceived = recv(sockfd, responseBuffer, sizeof(responseBuffer), 0);
        std::cout << "Received response: ";
        for (int i = 0; i < bytesReceived; ++i) {
            std::cout << responseBuffer[i];
        }
        std::cout << std::endl;
    }


    int main() {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Error: WSAStartup failed." << std::endl;
            return 1;
        }

        // Create socket
        SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET) {
            std::cerr << "Error: Unable to create socket." << std::endl;
            WSACleanup();
            return 1;
        }

        // Connect to strobe controller
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(30313);
        inet_pton(AF_INET, "192.168.1.26", &serverAddr.sin_addr);  // Change for the IP adress of your Strobe Controller
        if (connect(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Error: Unable to connect to strobe controller. Error code: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        // HPSC Commands
        unsigned char requestPacketHPSC[] = { 0x01, 0x41, 0x08, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x41, 0xCA, 0x5B, 0x04 }; // Change the Max Voltage to 15V 
        unsigned char requestPacketHPSC1[] = { 0x01, 0x42, 0x86, 0x68, 0x04 }; // Save User Parameters

        // IPSC Commands
        unsigned char requestPacketIPSC0[] = { '+', '\r' }; // Apply lock (Necessary before any command)
        unsigned char requestPacketIPSC[] = { "PT#0#5#10#200\rPN#0#1\rPT#1#2#10#200\rPN#1#0\rPO#0#100#1\rPC#0#8000\rPI#0#0\rPC#1#0\rPI#1#1\rPM#0#3\rSP\r" }; // Changing different parameters (View IPSC Raw commands manual)
        unsigned char requestPacketIPSC1[] = { '-', '\r' }; // Release lock (Necessary after any command)                                                          
                                                         

        // Send HPSC Commands
        sendPacketHPSC(sockfd, requestPacketHPSC, sizeof(requestPacketHPSC));
        sendPacketHPSC(sockfd, requestPacketHPSC1, sizeof(requestPacketHPSC1));

        // Send IPSC Commands
        //sendPacketIPSC(sockfd, requestPacketIPSC0, sizeof(requestPacketIPSC0));
        //sendPacketIPSC(sockfd, requestPacketIPSC, sizeof(requestPacketIPSC));
        //sendPacketIPSC(sockfd, requestPacketIPSC1, sizeof(requestPacketIPSC1));

        // Clean up
        closesocket(sockfd);
        WSACleanup();
        return 0;
    }
