#include "kernel/types.h"
#include "user/user.h"

// Dùng sàng Eratosthenes
void
parent(int outfd, int maxn)
{   
    // Đầu ra là outfd (file description)
    for (int i = 2; i <= maxn; ++i)
    {
        write(outfd, &i, 4);
        // 4 là 4 byte (32 bits) là kích thước của một biến kiểu int
    }
}

void
child(int infd)
{
    int btr;
    int n, prime;
    int p[2];

    // process first number
    btr = read(infd, &n, 4); // lưu kết quả trả về từ hàm read
    if (btr == 0)
    {
        return;
    }
    printf("prime %d\n", n);
    prime = n;

    // process the rest number
    pipe(p);
    if (fork() == 0) // tạo một quy trình con
    {
        close(p[1]);
        // Đóng đầu ghi của ống trong quá trình con. Quá trình con chỉ đọc dữ liệu từ ống, không ghi vào ống này.
        child(p[0]);
        // Gọi lại hàm child đệ quy với đầu đọc của ống làm đầu vào. Quá trình con tiếp tục xử lý các số từ đầu vào (p[0]).
        close(p[0]);
        // Đóng đầu đọc của ống trong quá trình con
        exit(0);
    }

    close(p[0]);
    while (read(infd, &n, 4)) 
    {
        // Nếu n là số nguyên tố
        if (n % prime == 0)
        {
            continue;
        }
        // ghi của ống p[1] để chuyển đến các quy trình con khác để tiếp tục xử lý
        write(p[1], &n, 4);
    }
    close(p[1]); // Sau khi tất cả các số đã được xử lý, đầu ghi của ống p[1]
    wait(0); // đợi cho đến khi tất cả các quy trình con kết thúc
}

int
main(int argc, char *argv[])
{
    int p[2]; // parent => child
    pipe(p);
    // parent
    if(fork()) 
    {
        close(p[0]);
        parent(p[1], 35);
        close(p[1]);
        wait(0);
    }
    // child
    else 
    {
        close(p[1]);
        child(p[0]);
        close(p[0]);
        exit(0);
    }
    exit(0);
}