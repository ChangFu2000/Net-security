/*
 * Man in the middle attack. Reference:
 * https://github.com/archit-p/simplest-oblivious-transfer
 */
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "DES.h"
#include "DH.h"
#include "main.h"

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 100

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Self-socket initialization
    SOCKET eve_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in eve_sock_addr;
    memset(&eve_sock_addr, 0, sizeof(eve_sock_addr));
    eve_sock_addr.sin_family = PF_INET;
    eve_sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    eve_sock_addr.sin_port = htons(4200);

    bind(eve_sock, (SOCKADDR*)&eve_sock_addr, sizeof(eve_sock_addr));
    listen(eve_sock, 20);

    // Accept requests from Alice
    SOCKADDR alice_addr;
    int nSize = sizeof(SOCKADDR);
    SOCKET alice_sock = accept(eve_sock, (SOCKADDR*)&alice_addr, &nSize);

    // Connect to Bob's socket
    SOCKET bob_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in bob_sock_addr;
    memset(&bob_sock_addr, 0, sizeof(bob_sock_addr));
    bob_sock_addr.sin_family = PF_INET;
    bob_sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bob_sock_addr.sin_port = htons(8888);

    connect(bob_sock, (SOCKADDR*)&bob_sock_addr, sizeof(SOCKADDR));

    struct s {
        int p;
        int g;
        int key;
    };

    // s_1 and s_2 are original message received from Alice and Bob
    struct s s_1, s_2;
    // s_c is the faked message generated by Eve
    struct s s_c;

    struct str {
        char str0[maxn];
        char str1[maxn];
    } message;

    // For simplicity, choose a shared random number for both Alice and Bob
    int r_c = ran();
    int p_c = p();
    int g_c = g(p_c);

    // k_a is the shared key with Alice
    // k_b is the shared key with Bob
    int k_a, k_b;

    char temp[sizeof(str)];
    memset(temp, 0, sizeof(temp));

    // Interecpt message sent from Alice, pretend to be Bob and send back his
    // public value
    recv(alice_sock, temp, sizeof(temp), 0);
    memcpy(&s_1, temp, sizeof(s));

    int hope = 0;
    int v_a;

    if (hope == 0) {
        v_a = MoChongFu(s_1.g, r_c, s_1.p);
    } else {
        v_a = MoChongFu(s_1.g, r_c, s_1.p) * s_1.key;
    }
    s_c.key = v_a;

    memset(temp, 0, sizeof(temp));
    memcpy(temp, &s_c, sizeof(s));
    send(alice_sock, temp, sizeof(temp), 0);

    printf("Fake g^b or Ag^b sent back to Alice: %d\n", v_a);

    // Pretend to be Alice and send his own public value to Bob
    int v_b = key(g_c, r_c, p_c);

    s_c.p = p_c;
    s_c.g = g_c;
    s_c.key = v_b;
    memset(temp, 0, sizeof(temp));
    memcpy(temp, &s_c, sizeof(s));

    send(bob_sock, temp, sizeof(temp), 0);
    memset(temp, 0, sizeof(temp));

    printf("Fake g^a sent to Bob: %d\n", v_b);

    // Intercept response sent from Bob, pretend to be Alice and send arbitrary
    // message back to Bob
    recv(bob_sock, temp, sizeof(temp), 0);
    memcpy(&s_2, temp, sizeof(s));

    int k0 = MoChongFu(s_2.key, r_c, p_c);
    int k1 = MoChongFu(s_2.key / v_b, r_c, p_c);

    memcpy(message.str0, final(k0), maxn);
    result(message.str0, k0);

    memcpy(message.str1, final(k1), maxn);
    result(message.str1, k1);

    memset(temp, 0, sizeof(temp));
    memcpy(temp, &message, sizeof(str));
    send(bob_sock, temp, sizeof(temp), 0);

    printf("Encrypted message sent to Bob.\nMessage 0: %s\nMessage 1:%s\n",
           message.str0, message.str1);

    closesocket(alice_sock);
    closesocket(bob_sock);
    closesocket(eve_sock);

    WSACleanup();
    system("pause");
    return 0;
}
