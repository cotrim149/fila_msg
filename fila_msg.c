/*
 *  exemplo de utilizacao de msgget()
 	usar esse comando pra visualizar: ipcs -q
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 0x123
#define SIZE_MSG 1000

enum type_msg {NORMAL,URGENTE};

typedef struct
{
	int type;
	char* text;
}MsgStruct;

int main()
{
	
	int msg_id ;  /* ID da fila de mensagens */
	
	/*
	 * criacao de uma fila de mensagens para leitura se
	 * ela ainda nao existe
	 */
	 if (( msg_id = msgget(KEY,IPC_CREAT|IPC_EXCL|0600)) == -1) {
		  perror("Erro de msgget") ;
		  return(-1) ;
	 }

	printf("identificador da fila: %d\n",msg_id) ;
	printf("esta fila esta associada a chave unica : %#x\n",KEY) ;
	
	// configurando msg
	MsgStruct msg;
	msg.type = URGENTE;
	msg.text = "Mensagem do Host A";
	
	int status_send = msgsnd(msg_id,&msg,sizeof(msg.text),0);
	
	if(status_send == -1)
		printf("Erro ao enviar msg!\n");
	else
		printf("Msg enviada com sucesso\n");
	
	MsgStruct msg_receive;
	
	int status_receive = msgrcv(msg_id,&msg_receive,sizeof(msg.text), NORMAL, 0);
	
	if(status_receive == -1)
		printf("Erro para receber msg!\n");
	else
		printf("Msg recebida com sucesso: %s\n",msg_receive.text);
	
//	int size_command = 100;
	char command[20];
	sprintf(command,"ipcrm -Q %x",KEY);
	printf("Command: %s\n\n",command);
	//execl("/bin/ls","",NULL,NULL);
	return(0);
}

