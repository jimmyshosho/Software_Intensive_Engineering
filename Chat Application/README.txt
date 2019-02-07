README.txt
Project 6: Simple Chat Application
Group: Dhimiter Shosho & Ibrahim Tahoun

SUMMARY OF CODE:
  The code provided implements a simple chat application that can theoretically support up to 100 users. It consists
of two parts, client.c and server.c. The server is first run (on port 8080) to initiate the server, and clients may
then connect to the server on the same port. Once a client connects, they are greeted by the terminal and are asked to
press ENTER to "connect" and chat with others. The client now has the <quit> and <list-users> commands at their dispose
as well as the ability to @ other users. When the client requests <list-users>, the command is sent to the server which
utilizes an if condition that concatenates every user in the users[] array than then uses Rio_writen(...) to give the
client a list of users on the server. If two or more users are in the server, they may utilize the @<user> command to
communicate with others. Given three users A, B, and C, user A may @ user C or user B by using the command @C <message>
or @B <message>. This is able to work effectively because the client is composed of two threads,a thread that just reads
from the sever and prints, and another than sends information to the server and takes stdin input. Lastly, a client
may use the <quit> command that leaves the server. When a client requests <quit>, the command is send to the server which
utilizes an if condition that first, prints that the user has left the server, then sends a "Goodbye!" back to the client.
The client has an if condition that is entered if and only if the server sends exactly "Goodbye!". This message is printed
onto the terminal and an exit(0) command is initiated to close the client.

SOLUTION APPROACH:
  We saw it fit to first implement the <quit> and <list-users> commands as they seemed more trivial than the actual
communication between different clients. After, we moved on to the more challenging part of messaging.
  The <quit> command is initiated when the clients enters "quit" into the terminal and presses enter. This command is
then sent to the server which catches it via an if condition. The server then loops through the userID[] array to find
the index of the user that called the <quit> command. It will find the user and print "<user> left" and will then send
exactly "Goodbye!" back to the client that requested <quit>. The client then uses an if condition that catches this
message, prints it out, closes the client, and calls exit(0). By now, the client that has requested to quit the server
is no longer in the system in the server and others may not communicate with this client anymore.
  Similarly, the <list-users> command is initiated when the client enters "list-users" into the terminal and presses enter.
The command is then sent to the server which utilizes an if condition to catch this command. The server then loops through
the users[], concatenating each string in every index into the buffer "buf". This is then sent back to the client using
Rio_writen(...) which prints the list of users onto the clients terminal.
  In order to successfully get clients to communicate with each other effectively, our clients had to be multi-threaded.
Each client is composed of two threads, one that calls readMSG() and one that calls sendMSG(). The responsibilities of
readMSG() consist of just reading and printing to the clients terminal whereas sendMSG() sends commands from stdin back
to the server to do said computations. To start, a client may message another client by using the @<user> <message>
format. If client A decides to message client B they would write something like @B Hello!. This is then sent to the server
upon pressing ENTER. The server catches the '@' character that goes into an if condition. It then loops through buf
storing "@B" into a temporary variable. It then loops through the users[] array to check if "@B" is indeed a user on
the server. If they are, it will successfully loop through the usersID[] array to get the connfd to send the message to.
It then sends "@A Hello!" to user B successfully. This algorithm is very effective and can support more than two clients.
  If the user did not type <list-users>, <quit> or @<validUser> <message>, the server had an if condition that would check
this and send a message back to the client telling them to type a valid command or communicate with a user that exists
on the server. This provided some robustness that would prevent the client from crashing due to a misspelled or incorrect
command.

PROBLEMS ENCOUNTERED:
  One significant problem we encountered while implementing this code was how to effectively concatenate and replace the "buf"
buffer. We did not utilize the strtok() command but instead used strcat() and strcpy() which seemed to do the trick. This was
crucial to get right off the bat because the whole program depended on effectively manipulating character arrays and character
pointers.
  While implementing <quit>, a problem that we faced was getting the client to close properly, along with printing a few messages
("user left" in the server and "Goodbye!" in the client) at the same time. This problem was quickly addressed once we realized
that we can send <quit> to the server, which will catch it with an if condition, then print "<user> left" and send exactly
"Goodbye!" to the client. The client would then catch this with an if condition that would print "Goodbye!" to the screen and
call Close(clientfd) and exit(0) to leave the server.
  A significant obstacle that needed to be overcome was incorporating a second thread for the client in order to allow for simultaneous
reading and writing from the server. When a client wrote to the server(directed towards another client), the client would then
enter a permanent read state, which meant it was stuck until given a response. On the other side of this, a client that didn't
write a message to deliver, was stuck in a write state, and so any incoming message would not appear until after something was
delivered from that client. To solve this, my partner and I used the pthread_create to have 2 separate loops running at all times,
one to read from the server and print it to client terminal, named readMSG, and the other to take input from stdin and output
it to the server. These concurrent loops enable the chat to run on a thread based execution.

PROBLEMS WITH CODE & DIFFERENTIATION FROM SPECIFICATIONS:
  Overall, the code runs rather smoothly without any major problems. One pesky thing we were never able to fix was correctly
printing out '>' to the client so they could type commands. It works sometimes but other times the user must type on the line
below the '>'. This is more of a legibility issue than anything else as it has no effect on the performance of the program.

END.
