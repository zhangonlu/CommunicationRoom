OBJ = main.o  initia_page.o cJSON.o login.o registe.o tcp.o online_write.o friends_fun.o
LIBS =-lpthread -lsqlite3
%.o : %.c
	$(CC) -c $< -o $@
main :  $(OBJ)
	gcc $^ -o $@ $(LIBS) 
clear :
	$(RM) *.o main
