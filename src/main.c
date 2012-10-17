
#include <stdio.h>
#include <pcaphelper.h>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

extern int Authentication(const char *UserName, const char *Password, const char *DeviceName);

int main(int argc, char *argv[])
{
	char *UserName = NULL;
	char *Password = NULL;
	char *DeviceName = NULL;
	char buf[256];

	if (argc < 3 || argc > 4) {
		char *appname;
#if defined(WIN32)
		int len, i;
		appname = argv[0];
		len = strlen(appname);

		for (i = len - 1; i > 0; i--) {
			if (appname[i] == '\\' || appname[i] == '/') {
				appname = appname + i + 1;
				break;
			}
		}
#else
		appname = argv[0];
#endif
		fprintf(stderr, "�����в�������\n");
		fprintf(stderr,	"��ȷ�ĵ��ø�ʽ�������£�\n");
		fprintf(stderr,	"    %s username password\n", appname);
		fprintf(stderr,	"    %s username password eth0\n", appname);
		fprintf(stderr,	"    %s username password eth1\n", appname);
		fprintf(stderr, "(ע������ָ��������Ĭ������½����������ѡ��)\n");
		exit(-1);
	} else if (argc == 4) {
		DeviceName = argv[3]; // �����������ָ���豸��
	} else {
		// ȱʡ�����ʹ�õ��豸
		char errbuf[PCAP_ERRBUF_SIZE];
		pcap_if_t *alldevs;
		pcap_if_t *dev;
		char key;

		if (pcap_findalldevs(&alldevs, errbuf) == -1) {
			fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
			exit(1);
		}

		printf("Select an adapter by input Y\n");

		for (dev = alldevs; dev != NULL; dev = dev->next) {
			printf("%s (%s)\n", dev->name, dev->description);
			key = getchar();
			if (key == 'Y' || key == 'y') {
				strcpy(buf, dev->name);
				DeviceName = buf;
				break;
			}
		}

		pcap_freealldevs(alldevs);
	}

	if (!DeviceName) {
		printf("Please select an adapter!\n");
		exit(-1);
	}

	UserName = argv[1];
	Password = argv[2];

	Authentication(UserName, Password, DeviceName);
	
	return 0;
}
