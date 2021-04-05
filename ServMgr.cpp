#include "ServMgr.h"




int main(int arg, char* argv[])
{
	char zn[50],zn1[150],zn2[50];
	WIN32_FIND_DATAA wfd;
	HANDLE hn=NULL;
	int i,d = 0;
	SC_HANDLE mgr, srv;
	SERVICE_STATUS ss;

	if (arg < 3 || arg>3)
	{
		printf("Blad polecania \nNalezy podac sciezke dostepu do pliku sterownika..\n");
		printf("Skladnia polecenia:\nSvcMgr -load/unload [sciezka dostepu do pliku/Nazwa uslugi]\n");
		scanf_s(zn);
	}
	else
	{
		strcpy_s(zn, argv[1]);
		strcpy_s(zn1, argv[2]);
		if (strcmp(zn, "-load") == 0)
		{
			hn = FindFirstFileA(zn1, &wfd);
			if (hn == INVALID_HANDLE_VALUE)
			{
				printf("Nie mozna odnalezc podanego pliku....\n");
				scanf_s(zn);
			}
			else
			{

				__try
				{
					i = CloseHandle(hn);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
				}
				i = d = (int)strlen(zn1);
				for (i=i; i > 0; i--)
				{
					if (zn1[i] != '\\') d--; else i = 0;
				}
				memset(zn, 0, 50);
				if (d > 0)i = d+1; else i = 0;
				d = 0;
				
				for (i = i; i < strlen(zn1); i++)
				{
					zn[d] = zn1[i];
					d++;
				}
				memset(zn2, 0, 50);
				for (i = 0; i < strlen(zn) && zn[i]!='.'; i++)
				{
					zn2[i] = zn[i];
				}

				mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
				if (mgr)
				{
					srv = CreateServiceA(mgr, zn2, zn2, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,zn1, NULL, NULL,NULL, NULL, NULL);
					if (srv == NULL)
					{
						printf("Nie mozna zaladowac uslugi...kod bledu:%d \n",GetLastError());
						scanf_s(zn);
					}
					else
					{
						printf("Zaladowano usluge pomyslnie...\n");
						CloseServiceHandle(srv);
					}
					CloseServiceHandle(mgr);
					scanf_s(zn);
				}
				else
				{
					printf("Nie mozna otworzyc menadzera uslug....\n");
					scanf_s(zn);
				}
			}
		}
		else if (strcmp(zn, "-unload") == 0)
		{
			mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (mgr != NULL)
			{
				srv = OpenServiceA(mgr, zn1, SERVICE_ALL_ACCESS);
					if (srv != NULL)
					{
						printf("Zamykam usluge");
						i = 0;
						d = 0;
						if (ControlService(srv, SERVICE_CONTROL_STOP, &ss))
						{
							while (QueryServiceStatus(srv, &ss))
							{
								if (ss.dwCurrentState == SERVICE_STOP_PENDING)
								{
									if (i < 2) { printf("."); i++; }
									else
									{
										for (i = 0; i <= 2; i++) printf("%c", 8);
										i = 0;
									}
									d++;
									Sleep(500);
									if (d == 100) break;
								}
								else break;
							}
							if (ss.dwCurrentState == SERVICE_STOPPED) printf("\nUsluga zostala zamknieta\n");
							else printf("\nNie mozna zamknac uslugi...\n");
						}
						if (DeleteService(srv)) printf("\nUsluga zostala usunieta pomyslnie...\n");
						else printf("\nNie mozna usunac uslugi.....\n");
						CloseServiceHandle(srv);
					}
					else
					{
						printf("Nie mozna otworzyc uslugi o podanej nazwie...\n");
						scanf_s(zn);
					}

					CloseServiceHandle(mgr);
					scanf_s(zn);
			}
			else
			{
				printf("Nie mozna otworzyc menadzera uslug...\n");
				scanf_s(zn);
			}
		}
	}
	return 0;
}