/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashAndSign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Vishnu <vishnu44d@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 01:00:11 by Vishnu            #+#    #+#             */
/*   Updated: 2020/06/03 01:08:53 by Vishnu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MD4.h"
#include "RSA.h"

int main(int argc, char const *argv[])
{
    FILE *fd;
    int length;
    char *message;
    fd = fopen("KYC.txt", "r");
    message = readFileMD4Input(fd, &length);
    fclose(fd);
    printf("Read message from file: %s\nlength: %d\n", message, length);
    char *digest;
    digest = MD4(message, length);
    printf("Message digest from MD4 algorithm: %s\n", digest);

    fd = fopen("digest.txt", "w+");
    fprintf(fd, "%s", digest);
    fclose(fd);

    FILE *fp;
    // char *str_d = "", *str_n = "";
    char *str_d = (char *)malloc(200 * sizeof(char));
    char *str_e = (char *)malloc(200 * sizeof(char));
    char *str_n = (char *)malloc(200 * sizeof(char));
    size_t len = 0;
    int byte = 94;

    fp = fopen("privatekey.txt", "r");
    read_key_file(fp, &str_d, &str_n);
    fclose(fp);

    fp = fopen("publickey.txt", "r");
    read_key_file(fp, &str_e, &str_n);
    fclose(fp);

    i128 *e = i128_init();
    i128 *d = i128_init();
    i128 *n = i128_init();

    from_str_to_i128(e, str_e);
    from_str_to_i128(d, str_d);
    from_str_to_i128(n, str_n);
    printf("e is: ");
    print_i128(e);
    printf("\n");
    printf("d is: ");
    print_i128(d);
    printf("\n");
    printf("n is: ");
    print_i128(n);
    printf("\n");

    int in_len;
    char *buffer;
    int bytes = 94;

    // Now signing this digest with private key
    fp = fopen("digest.txt", "r");
    in_len = readFile(fp, &buffer, bytes);
    fclose(fp);
    printf("\nMessage is (D): %s\n", buffer);
    // signing the message, that is already encrypted using RSA.
    // with my private key (d, n).
    // I am Alice Here. who wants to send the message along with it's signature.
    i128 *sign = i128_init();
    sign = encodeMessage(in_len, bytes, buffer, d, n);
    printf("Signature is: ");
    print_i128(sign);
    printf(" \n");
    // writing this signature to a signature file.
    fp = fopen("signature.txt", "w+");
    fprint_i128(fp, sign);
    fclose(fp);

    // This signature.txt and cipher.txt is sent to BOB.
    // Here I am bob.
    // I will decrypt the signature using Alice's Public key (e, n)
    // and if this message is equals to the message I will accept otherwise reject.
    char *sign_buffer;
    int s_len;
    fp = fopen("signature.txt", "r");
    s_len = readFile(fp, &sign_buffer, bytes);
    fclose(fp);
    i128 *signature = i128_init();
    from_str_to_i128(signature, sign_buffer);
    printf("File se read signature: ");
    print_i128(signature);
    printf("\n");
    char *m_dash;
    m_dash = decodeMessage(in_len / bytes, bytes, sign, e, n);
    printf("Decrypted Message is (D`): %s\n", m_dash);

    // comparing m` with actual message stored in buffer
    if (!strcmp(buffer, m_dash))
    {
        printf("D and D` is same. Accepting the message\n");
    }
    else
    {
        printf("D and D` is different. Rejecting the message.");
    }

    i128_free(e);
    i128_free(d);
    i128_free(n);
    i128_free(sign);
    i128_free(signature);

    // free(str_n);
    // free(str_d);
    str_e = NULL;

    return 0;
}