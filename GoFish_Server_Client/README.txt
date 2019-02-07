README.TXT
Project 5: Gofish with client server communication
Group: Dhimiter Shosho & Ibrahim Tahoun
*CLIENT HARD CODED TO RUN ON PORT 8080*
*RUN SERVER ON PORT 8080*

PROGRAM SUMMARY:

  This project was to be an addition to the prior Go Fish project, now adding a client and server single thread network.
The project begins by running the server first, which will prompt the server to begin listening (listening on port
8080) for a client to be run, in search of the port passed as an argument (which should be same as that on server).
After the client has found the server, the server will stop listening for other clients as this program is only a single thread
network. The user will be playing as player 1 against the computer, the client side of the program
will take in your card request as input, and send your input back to the server using Rio_writen(). The server
will be ready to take in this input, which is all that is required for player 1's turn.
The server will handle the card request, status if the request (whether the opponent had it or not),
and act accordingly. After this, the opponent's turn will also occur within the server, and as the turn progresses,
the server will write back the output of the Go Fish program to the client, for the user to see, and
enter their next card request. All of the code for the Go Fish program has been included or pasted
in the assignment5 folder. The Make File was also edited to give server.c the ability to compile with player.c
and deck.c in order to use the functions properly.

  Noticing that the threading and communication between the server and client had already been implemented to the
server-client echo program, we decided that the best approach would be to incorporate the main method of
our Go Fish program, step by step into the echo method on the server to handle the majority of the game play,
and have it write back to the client for the user to see. The client side would then need to read the
incoming messages from the server so that the user can see the game progress, and also prompt the user for
input card request.

	As we began to write the program, we realized that strings can't simply be sent back to
the client, nor can we send back strings with variable placeholders. In order to overcome this, we employed
the STRCPY() method to overwrite the <buf> character array variable every time we began a new string to be written back
to the client. In many situations we found ourselves in need of concatenating more information into <buf> and we
couldn't simply add variables to the string. To solve this, we utilized STRCAT() to concatenate more strings alongside
other chunks of each string, piece by piece, and only writing back to the client once the string was fully
constructed. This meant creating local char[] variables that we would assign our card choices, then STRCAT()
it to our string.

	Once we were able to reliably write information using our STRCPY(), STRCAT(), and Rio_writen() line
combinations in the server echo function, we faced a new challenge that arose on the client terminal.
The spacing between sections of text that was being printed back to the client appeared to change at strange
increments, placing what seemed to be <\n> where we didn't suspect would occur, and in pressing <Enter> to resume,
would display errors on the server terminal and would sometimes display nothing at all in the client terminal.
To start, the server terminal would loop infinitely, which was quickly fixed by having it only print once per
faulty <Enter>. This made it much easier to understand what was going on with some of the other errors we had.
In order to solve the strange spacing occurrence in the client terminal, we eventually figured out that the client.c
program needed to be reading the correct number of lines EVERY time the server would write back to it, not
only once through the loop, but even at the end of the loop through the beginning of the next iteration.
This meant that the client would need to be prepared to read in a different number of lines at the beginning
of the program, versus the remainder of it. Our solution was to go into our echo method, and for each
separate IF statement that wrote back to the client, we drew a map of the different paths the echo method
took within one iteration of the loop, counting the number of new lines <\n> per IF/ELSE chunk. We then went into each
IF/ELSE chunk and added the required number of <\n>'s in each of them so that at the end of each loop, any path
taken due to different events of the game always had the same number of  new lines written back to the client.
Once we were able to consolidate them to the same number of lines, we added to possible reading options on the
client.c program, one for the beginning of the program, and one for the remainder of it. By adding a flag
variable initialized at 0, we would check its value, and if it was 0, the client would read the number of
lines needed for the beginning, and set flag to 1. For the remainder of loops through the program, client would
only be reading the mid-program number of lines as it would execute the <flag!=0> portion of the client code.

Once we figured out the <\n> problem involving the Rio_readlineb() and Rio_writen() functions and well as the
<buf> string concatenating problem that we struggled through the most, we utilized the printf() function to do most
the game logic debugging. Once in a while, if the computer wins, the server might crash due to a *Floating point exception
error that we believe is most likely coming from somewhere inside the player.c logic of the program. We did not
bother fixing this issue because Go Fish functions perfectly 95% of the time and we believed that the original
program was sufficient enough to give it client-server functionality.

*To play again, the user must reconnect to the server

*When the client is connected to the server, the user is prompted to press <ENTER> to start the game. The user should
press <ENTER> on the keyboard and the game will start.

*The corresponding code will need one argument to run client.c (just the <host> and no port argument is necessary).

*When running server.c, always set port argument to 8080, as this specific port has been hard coded into client.c.

*It is worth noting that the program runs with case sensitivity (e.g. capital letter arguments for card requests).

*The <10> card is "0" and Ace is "A".
