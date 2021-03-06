# Assignmenet 5

github: https://github.com/Vishnu44d/iss/tree/master/ass5

## Problems

- ## Part 1

  1.  Generate two 128-bit prime numbers p and q (generate random 128-bit numbers and test their
      primality. Be cautious about pseuso-primes!)

  2.  Compute n = p×q and Ø(n) subsequently

  3.  Select any e such that: 1<e< Ø(n) and GCD(e, Ø(n)) = 1 (Use Euclidean Algorithm here, generate
      e randomly in the valid range)

  4.  Compute d = e-1Ø(n) (Use Extended Euclidean Algorithm)

  Now use the parameters e, d and n appropriately to encrypt and decrypt English text using RSA
  encryption and decryption algorithms respectively. (Use the 00-26 encoding to represent the English
  letters as numbers.)

- ## Part 2

  1.  Consider any valid KYC document of yours. (Might be Aadhaar card, voter id card, etc.)

  2.  Scan it into an image.

  3.  Convert the image into binary form (may use the 00-26 encoding)

  4.  Generate digital signature for your binarized KYC document, using RSA, and prepend it to
      your document.

  5.  Pose now as a different user, the recipient, who has your public key and receives the
      document attached with the signature. Verify the signature as this other user.

## Solutions

- ## Part 1

  1. RSA.c contains the code for generating RSA key pairs, Encoding a message and Decoding a cipher txt.

  2. Asume that the flow is in Alice's machine.

  3. The code assumes a files (message.txt) is present in the same directory, that contains the message in plain simple english text.

  4. The code produces these files.

     1. privatekey.txt -> It contains the private key generated by RSA algorithm. The second line of file contains the private key (d, n).

     2. publickey.txt -> It contains the public key generated by RSA algorithm. The second line of file contains the public key (e, n).

     3. cipher.txt -> It contains the encrypted message encrypted using public key (e, n).

     4. plain.txt -> It contains the decoded message, decoded by RSA algorithm using the private key (d, n).

  5. To run the code.

     ```bash
     1. gcc -o RSA RSA.c
     2. ./RSA
     ```

  6. After generating RSA key pairs. It assumes that the public key is distributed and now code is running in different user. (Just for demonstration purpose, let us say this new user is Bob). After this, the Bob reads the message.txt and encode this message using the public key (e, n). And sends the cipher.txt back to Alice. And now alice decrypt the cipher.txt using its' private key (d, n) and produces plain.txt.

- ## Part 2

  1. RSAsignature.c contains the codes for the signature.

  2. First assume that the code is running on Alice's machine.

  3. The code first loads the private key from the privatekey.txt file.

  4. Then it reads the KYC.txt file, and encodes it into cipherKYC.txt.

  5. Now it sign the cipherKYC.txt with RSAsignature and proudes a file signature.txt.

  6. These two files, cipherKYC.txt and signature.txt is sent to Bob.

  7. Now assume the flow is in Bob's machine.

  8. It decrypt the signature.txt using Alice's public key (e, n).

  9. It reads the cipherKYC.txt. If the contents of cipherKYC.txt and decrypted signature.txt (in previous step) is same, It ccepts the message otherwise rejects the message.

  10. To run the code

  ```bash
  1. gcc -o Sign RSAsignature.c
  2. ./Sign
  ```
