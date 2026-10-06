#ifndef FLAGS_H
#define FLAGS_H

typedef struct
{
    int flag_A; // -A: Liệt kê trừ . và ..
    int flag_a; // -a: Liệt kê tất cả
    int flag_c; // -c: Dùng ctime
    int flag_d; // -d: Liệt kê bản thân thư mục
    int flag_F; // -F: Thêm ký tự đánh dấu kiểu file (/, *, @...)
    int flag_f; // -f: Không sắp xếp
    int flag_h; // -h: Kích thước dễ đọc (KB, MB...)
    int flag_i; // -i: In số inode
    int flag_k; // -k: Kích thước theo KB
    int flag_l; // -l: Định dạng dài (Long format)
    int flag_n; // -n: Long format với UID/GID dạng số
    int flag_q; // -q: Ẩn ký tự không in được bằng '?'
    int flag_R; // -R: Liệt kê đệ quy
    int flag_r; // -r: Đảo ngược thứ tự sắp xếp
    int flag_S; // -S: Sắp xếp theo kích thước
    int flag_s; // -s: In số block
    int flag_t; // -t: Sắp xếp theo thời gian
    int flag_u; // -u: Dùng atime
    int flag_w; // -w: In thô ký tự không in được
} Options;

void init_options(Options *opts);
int parse_flags(int argc, char *argv[], Options *opts, int *opt_ind);

#endif // FLAGS_H