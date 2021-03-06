# Setting up a trusted network

## problem statement.

1. Set up a network with 5 users and a Trusted Third Party (TTP), whom all users trust. Each user and
   the TTP holds a private, public RSA key pair. (Generate the keys for all 6 nodes, and broadcast all
   public keys within the network.)

2. Every user has to register with the TTP before she can start communication over the network.
   (Assume password based registration and authentication. Every user is assigned a unique id, and a
   password generated by the TTP, which she would use for subsequent logging in before using the
   network).

3. For reliable transmission of a message from user A to user B, first the TTP verifies A, for which A has
   to successfully login.

4. Then, A sends the message, sender id, and receiver id to the TTP, along with her digital signature.
5. The digital signature is verified by the TTP, and a copy of the message is saved with the TTP, so that
   later A cannot deny sending it.

6. Extracting information about the receiver (B’s id), the TTP now creates its own signature from the
   message, and sends the message, sender id, receiver id, the new signature and a timestamp to B.

7. B verifies the message using the public key of the TTP.

## Solution design

1. Different folders in this directory contains the information about that server/client. Each directory can be treated as they are running on different machines.

2. Server has the public key of all clients and its' own pair of private and public key.

3. On connection with a new client. Server asks for a password. If the client provides the password. Then it accepts the connection. Otherwise reject the connectiona and close the current socket.

4. After being accepted by the server client publisehes its' public key.

5. The server then sends its' own public key. So that the new user can send this key to communicate with server.

6. Now If client 1 wants to sends something to client 2. The following steps will be performed.

   1. Client 1 will encrypt the message with the public key of server. [produsing the file cipher.txt]

   2. Client 1 will sign this encypted message with its' own private key. [produsing the file signature.txt]

   3. Client 1 will send these two files (cipher.txt &signature.txt) to server.

   4. Server will recieve these file (cipher.txt and signature.txt).

   5. Server will decode the signature.txt by client 1's public key. And compare the decoded message with the contenet of cipher.txt. If both are not same then it will discard the message. Otherwise perform the next steps.

   6. Server will decode the cipher using its' private key.

   7. Server will encrypt the message with client 2's public key.

   8. Server will sign this encrypted message with its own private key.

   9. Server will write a entry of this transaction in its' log and then send these two files to cleint 2.

   10. Client 2 will recieve these two files. Client 2 will decode the signature using server's public key.

   11. Client 2 will compare the decode signature with recieved cipher. If both are not same then discards it. Otherwise performs the following steps.

   12. Client 2 will decrypt the cipher using the its' private key.

   13. Client 2 got the message.
