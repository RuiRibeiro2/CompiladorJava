declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)

@.fmt_int = private unnamed_addr constant [3 x i8] c"%d\00"
@.fmt_double = private unnamed_addr constant [6 x i8] c"%.16e\00"
@.fmt_str = private unnamed_addr constant [3 x i8] c"%s\00"
@.str_true = private unnamed_addr constant [5 x i8] c"true\00"
@.str_false = private unnamed_addr constant [6 x i8] c"false\00"


define i32 @factorial__i(i32 %arg0) {
  %v_n = alloca i32
  store i32 %arg0, i32* %v_n
  %t1 = load i32, i32* %v_n
  %t2 = icmp eq i32 %t1, 0
  br i1 %t2, label %L1, label %L2
L1:
  ret i32 1
L2:
  br label %L3
L3:
  %t3 = load i32, i32* %v_n
  %t4 = load i32, i32* %v_n
  %t5 = sub i32 %t4, 1
  %t6 = call i32 @factorial__i(i32 %t5)
  %t7 = mul i32 %t3, %t6
  ret i32 %t7
}

define i32 @main(i32 %argc, i8** %argv) {
  %v_argument = alloca i32
  %v_args = alloca i8**
  store i8** %argv, i8*** %v_args
  store i32 7, i32* %v_argument
  %t1 = load i32, i32* %v_argument
  %t2 = call i32 @factorial__i(i32 %t1)
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %t2)
  ret i32 0
}

