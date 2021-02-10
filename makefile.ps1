param (
    [string]$filename = $(throw "parameter missing: -name filename"),
    [string]$output = $(throw "parameter missing -name output"),
    [string]$include
)

if ($include) {
    g++ -O3 -g -std=c++2a -I E:\thirdparty\INCLUDE -I $include -L E:\thirdparty\LIB\ -lglm_static -o $output $filename -fopenmp
}
else {
    g++ -O3 -g -std=c++2a -I E:\thirdparty\INCLUDE -L E:\thirdparty\LIB\ -lstatic_glm -o $output $filename -fopenmp
}