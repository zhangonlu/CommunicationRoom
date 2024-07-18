OBJ = main.o client.o  initia_page.o cJSON.o login.o registe.o  main_function_page.o online_write.o
%.o : %.c
	$(CC) -c $< -o $@
main :  $(OBJ)
	gcc $^ -o $@ $(LIBS) 
clear :
	$(RM) *.o main
