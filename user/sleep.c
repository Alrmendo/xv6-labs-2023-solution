#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[]) // Điểm nhập của chương trình với đối số dòng lệnh
{
    // Kiểm tra xem số lượng đối số được cung cấp có khác 2 không
    if (argc != 2) 
    {
        fprintf(2, "Usage: sleep + [time]\n");
    }
    int time = atoi(*++argv);
    // Chuyển đổi biểu diễn chuỗi của thời gian thành số nguyên bằng cách sử dụng hàm atoi
    // Tăng con trỏ argv để trỏ đến đối số tiếp theo (thời gian) và giải tham chiếu nó để lấy giá trị
    // Lưu trữ giá trị thời gian đã chuyển đổi vào biến 'time'
    if (sleep(time) != 0) 
    {
        fprintf(2, "Error in sleep sys_call!\n");
    }
    exit(0); // Thoát khỏi chương trình 0 (thực thi thành công)
}