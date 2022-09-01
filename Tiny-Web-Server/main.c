#include "tiny_err.h"

int main(int argc, char *argv[])
{
    err_init(1);
    LOGGER("测试输出日志：%d", 1);
    err_exit();
    return 0;
}