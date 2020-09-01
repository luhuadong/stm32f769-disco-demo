#include <rtthread.h>

#include <sys/socket.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include "netdb.h"

#define DEBUG_UDP_CLIENT

#define DBG_TAG               "UDP"
#ifdef DEBUG_UDP_CLIENT
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO /* DBG_ERROR */
#endif
#include <ulog.h>

static int started = 0;
static int is_running = 0;
static char url[256];
static int port = 8888;
static int count = 0;
//const char send_data[] = "This is UDP Client from RT-Thread.\n"; /* 发送用到的数据 */
static char recv_data[1024];

static void udpclient(void *arg)
{
    int sock, result;
    struct hostent *host;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

#if 0
    /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析） */
    host = (struct hostent *) gethostbyname(url);
    if (host == RT_NULL)
    {
        LOG_E("Get host by name failed!");
        return;
    }
#endif

    /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        LOG_E("Create socket error");
        return;
    }

#if 0
    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
#else
    /* 初始化服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    /* 绑定socket到服务端地址 */
    if (bind(sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        LOG_E("Unable to bind");
    }
#endif
    started = 1;
    is_running = 1;

    /* 总计发送count次数据 */
    //while (count && is_running)
    while (1)
    {
#if 0
        LOG_I("UDP send ...");
        /* 发送数据到服务远端 */
        sendto(sock, send_data, rt_strlen(send_data), 0,
               (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        /* 线程休眠一段时间 */
        rt_thread_mdelay(1000);

        /* 计数值减一 */
        //count --;
        count = 0;
        break;
#else
        /* int recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen); */
        //recvfrom(sock, recv_data, sizeof(recv_data), 0, );
        result = recvfrom(sock, recv_data, sizeof(recv_data), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (result < 0)
        {
            continue;
        }
        count++;
        rt_kprintf("%d\n", count);
#endif
    }

    if (count == 0)
    {
        LOG_I("UDP client send data finished!");
    }

    /* 关闭这个socket */
    if (sock >= 0)
    {
        closesocket(sock);
        sock = -1;
    }
    started = 0;
    is_running = 0;
}

static void usage(void)
{
    rt_kprintf("Usage: udpclient -h <host> -p <port> [--cnt] [count]\n");
    rt_kprintf("       udpclient --stop\n");
    rt_kprintf("       udpclient --help\n");
    rt_kprintf("\n");
    rt_kprintf("Miscellaneous:\n");
    rt_kprintf("  -h           Specify host address\n");
    rt_kprintf("  -p           Specify the host port number\n");
    rt_kprintf("  --cnt        Specify the send data count\n");
    rt_kprintf("  --stop       Stop tcpclient program\n");
    rt_kprintf("  --help       Print help information\n");
}

static void udp_recv_test(int argc, char** argv)
{
    rt_thread_t tid;
#if 0
    if (argc == 1 || argc > 7)
    {
        LOG_I("Please check the command you entered!\n");
        goto __usage;
    }
    else
    {
        if (rt_strcmp(argv[1], "--help") == 0)
        {
            goto __usage;
        }
        else if (rt_strcmp(argv[1], "--stop") == 0)
        {
            is_running = 0;
            return;
        }
        else if (rt_strcmp(argv[1], "-h") == 0 && rt_strcmp(argv[3], "-p") == 0)
        {
            if (started)
            {
                LOG_I("The tcpclient has started!");
                LOG_I("Please stop tcpclient firstly, by: tcpclient --stop");
                return;
            }

            if (argc == 7 && rt_strcmp(argv[6], "--cnt") == 0)
            {
                count = atoi(argv[7]);
            }

            if (rt_strlen(argv[2]) > sizeof(url))
            {
                LOG_E("The input url is too long, max %d bytes!", sizeof(url));
                return;
            }
            rt_memset(url, 0x0, sizeof(url));
            rt_strncpy(url, argv[2], rt_strlen(argv[2]));
            port = atoi(argv[4]);
        }
        else
        {
            goto __usage;
        }
    }
#endif
    tid = rt_thread_create("udp_client",
        udpclient, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/4, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    LOG_D("udp_client is running");
    return;

__usage:
    usage();
}

#ifdef RT_USING_FINSH
MSH_CMD_EXPORT(udp_recv_test, Recv from a udp socket.);
#endif
