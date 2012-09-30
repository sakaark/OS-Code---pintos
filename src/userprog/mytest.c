#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef long long int mqd_t;

typedef struct mq_attr {
long mq_flags;
long mq_maxmsg;
long mq_msgsize;
long mq_curmsgs;
}mq_attr;

typedef struct msg_in {
char *cnt;
int msg_len;
int msg_prio;
struct msg_in *prev;
struct msg_in *next;
}msg_in;

typedef struct mq {
char name[100];
mq_attr attr;
msg_in *head;
msg_in *tail;
}mq;

typedef struct mq_in {
mq queue;
struct mq_in *prev;
struct mq_in *next;
}mq_in;

typedef struct mq_list {
mq_in *head;
mq_in *tail;
}mq_list;

typedef struct mq_opened {
int oflag;
mq_in *queue;
struct mq_opened *prev;
struct mq_opened *next;
}mq_opened;

typedef struct openmq_list {
mq_opened *head;
mq_opened *tail;
}openmq_list;


////////////////////////////////////////////////////////////////////////////////////////

openmq_list b;
mq_list c;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

mqd_t sys_mq_open(const char *name, int oflag);
int sys_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
size_t sys_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
int sys_mq_close(mqd_t mqdes); 
int sys_mq_unlink(const char *name);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int sys_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio) {

mq_opened *mq_pnt = (mq_opened *)mqdes;
mq_opened *t; 
int flag=0;
t = b.head;

while(t != NULL)
{
	if(mq_pnt==t)
	{
		flag=1;
		break;
	}
	else
	{
		t=t->next;
	}		
}
if(flag==0)
{
printf("EBADF : descriptor specified in mqdes is invalid \n");
return (-1);
}

int oflag = mq_pnt->oflag;
int flag1 = oflag/100;
int flag2 = (oflag%100)/10;
int flag3 = (oflag%100)%10;

mq *msq = &((mq_pnt->queue)->queue);
if((msq->attr).mq_maxmsg==(msq->attr).mq_curmsgs)
{
	printf("EAGAIN: message queue already full \n");
	return (-1);
}

if((msq->attr).mq_msgsize < msg_len)
{
	printf("EMSGSIZE : message too long for the queue \n");
	return (-1);
}
if(flag2==1)
{
	printf("ERROR : Reading only open. Cant write in queue \n");
	return (-1);
}

msg_in *new = (msg_in *)malloc(sizeof(msg_in));
new->cnt = msg_ptr; //possible error
new->msg_len = msg_len;
new->msg_prio = msg_prio;

(msq->attr).mq_curmsgs = (msq->attr).mq_curmsgs + 1;

msg_in *tmsg;
tmsg = msq->head;
flag = 0;

if(tmsg == NULL)
{
	msq->head = new;
	msq->tail = new;
	new->next = NULL;
	new->prev = NULL;
	return 0;
}

while(tmsg != NULL)
{
	if(tmsg->msg_prio <= msg_prio)
		tmsg = tmsg->next;
	else
	{
		if(tmsg == msq->head)
		{
			msq->head = new;
			new->prev = NULL;
			new->next = tmsg;
			tmsg->prev = new;
			flag = 1;
			return 0;
		}
		else
		{	
			(tmsg->prev)->next = new;
			new->prev = tmsg->prev;
			new->next = tmsg;
			tmsg->prev = new;
			flag = 1;
			return 0;
		}
		
	}
}

if(flag==0)
{
	(msq->tail)->next = new;
	new->next = NULL;
	new->prev = msq->tail;
	msq->tail = new;
	return 0;
}
}
//////////////////////////////////////////////////////////////////////////////////////

size_t sys_mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio)
{
mq_opened *mq_pnt = (mq_opened *)mqdes;
mq_opened *t; 
int flag=0;
int i;
t = b.head;

while(t != NULL)
{
	if(mq_pnt==t)
	{
		flag=1;
		break;
	}
	else
	{
		t=t->next;
	}		
}
if(flag==0)
{
	printf("EBADF : descriptor specified in mqdes is invalid \n");
	return (-1);
}

int oflag = mq_pnt->oflag;
int flag1 = oflag/100;
int flag2 = (oflag%100)/10;
int flag3 = (oflag%100)%10;

mq *msq = &((mq_pnt->queue)->queue);
if((msq->attr).mq_curmsgs==0)
{
	printf("EAGAIN: message queue empty \n");
	return (-1);
}

if((msq->attr).mq_msgsize > msg_len)
{
	printf("EMSGSIZE : message too long  for the buffer \n");
	return (-1);
}
if(flag2==2)
{
	printf("ERROR : Writing only open. Cant Read in queue \n");
	return (-1);
}

(msq->attr).mq_curmsgs = (msq->attr).mq_curmsgs - 1;

msg_in *tmsg;
tmsg = msq->head;
if(tmsg->next != NULL)
{
	msq->head = tmsg->next;
	(tmsg->next)->prev = NULL;
}
else
{
	msq->head = NULL;
	msq->tail = NULL;
}

strlcpy(msg_ptr,tmsg->cnt, 1000);

if(msg_prio !=NULL)
{
	msg_prio[0]=tmsg->msg_prio;
}

return (tmsg->msg_len);
}
//////////////////////////////////////////////////////////////////////////////////////

int sys_mq_close(mqd_t mqdes)
{
mq_opened *mq_pnt = (mq_opened *)mqdes;
mq_opened *t; 
int flag=0;
t = b.head;

while(t != NULL)
{
	if(mq_pnt==t)
	{
		flag=1;
		break;
	}
	else
	{
		t=t->next;
	}		
}
if(flag==0)
{
	printf("EBADF : descriptor specified in mqdes is invalid \n");
	return (-1);
}
if(mq_pnt==b.head)
{
	b.head = mq_pnt->next;
	if(mq_pnt->next != NULL)
	(mq_pnt->next)->prev = NULL;
	return 0;
}
if(mq_pnt==b.tail)
{
	b.tail = mq_pnt->prev;
	if(mq_pnt->prev != NULL)
	(mq_pnt->prev)->next = NULL;
	return 0;
}
{
	(mq_pnt->prev)->next = mq_pnt->next;
	(mq_pnt->next)->prev = mq_pnt->prev;
	return 0;
}

}

///////////////////////////////////////////////////////////////////////////////////////

int sys_mq_unlink(const char *name)
{
if(strlen(name) > 100)
{
	printf("ENAMETOOLONG:name too long \n");
	return (-1);
}
mq_in *t;
int flag = 0;
t = c.head;
while(t != NULL)
{
	if(!(strcmp(name,(t->queue).name)))
	{
		flag = 1;
		break;
	}
	else
	{
		t = t->next;
	}
}

if(flag == 0)
{
	printf("ENOENT : there is no message queue with the given name \n");
	return -1;
}
if(t==c.head)
{
	c.head = t->next;
	if(t->next != NULL)
	(t->next)->prev = NULL;
	return 0;
}
if(t==c.tail)
{
	c.tail = t->prev;
	if(t->prev != NULL)
	(t->prev)->next = NULL;
	return 0;
}
{
	(t->prev)->next = t->next;
	(t->next)->prev = t->prev;
	return 0;
}

}
////////////////////////////////////////////////////////////////////////////////////////

mqd_t sys_mq_open(const char *name, int oflag)
{

//printf("%s \n",name);
if(strlen(name) > 100)
{
	printf("ENAMETOOLONG:name too long \n");
	return ((mqd_t)-1);
}

int flag1 = oflag/100;
int flag2 = (oflag%100)/10;
int flag3 = (oflag%100)%10;


mq_in *t;
int flag = 0;
t = c.head;
//printf("%lld \n",c.tail);
while(t != NULL)
{
	if(!(strcmp(name,(t->queue).name)))
	{
		flag = 1;
		break;
	}
	else
	{
		t = t->next;
	}
}

if(flag==1)
{
	if(flag1==2)
	{
		printf("EEXIST : Queue with given name already exist \n");
		return ((mqd_t)-1);
	}
}

if(flag1 == 0)
{
	printf("ENOENT \n");
	return (mqd_t)-1;
}
	
mq queue;
mq_in *queue_in = (mq_in *)malloc(sizeof(mq_in));
if(flag == 0)
{
	strlcpy(queue.name, name, 100);
	queue.head = NULL;
	queue.tail = NULL;
	queue.attr.mq_curmsgs = 0;
	queue.attr.mq_maxmsg = 100;
	queue.attr.mq_msgsize = 1000;
	queue.attr.mq_flags = 0;
	
	queue_in->queue = queue;
	queue_in->prev = c.tail;
	queue_in->next = NULL;
	if(c.tail == NULL)
	{
		c.head = queue_in;
	}
	else
	{
		(c.tail)->next = queue_in;
	}
	c.tail = queue_in; 
	t= queue_in;
}

mq_opened *pnt = (mq_opened *)malloc(sizeof(mq_opened));
{
	pnt->oflag = oflag;
	pnt->queue = t;
	pnt->prev = b.tail;
	pnt->next = NULL;
	if(b.tail == NULL)
		b.head = pnt;
	else
		(b.tail)->next = pnt; 
	b.tail = pnt;
	return ((mqd_t)pnt);
}
}

////////////////////////////////////////////////////////////////////////////////////////
































