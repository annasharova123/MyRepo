#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

printf "apple\nAPPLE\n   apple with spaces\n\nBANANA\ncherry\nlast line no newline" > test_complex.txt
printf "apple in second file\nno match here\n" > test_second.txt
printf "apple\ncherry\n" > test_patterns.txt

MISSING_FILE="nonexistent_file.txt"
FILE="test_complex.txt"
FILE2="test_second.txt"
PATTERN="apple"
PATTERN_FILE="test_patterns.txt"
MY_GREP="./s21_grep"
LOG_FILE="grep_test_$(date +%Y%m%d_%H%M%S).log"

# Флаги без -e, -f, -o (обрабатываются отдельно)
flags=("i" "v" "n" "c" "l" "s" "h")

echo "========================================"
echo "  STARTING ALL PAIR TESTS"
echo "  Log: $LOG_FILE"
echo "========================================"

PASS=0
FAIL=0

log() {
    echo "$1" >> "$LOG_FILE"
}

run_test() {
    local label="$1"
    local grep_cmd="$2"
    local mygrep_cmd="$3"

    eval "grep $grep_cmd > orig.txt 2>&1"
    eval "$MY_GREP $mygrep_cmd > mine.txt 2>&1"

    sed -i '' 's/^[^:]*: /PROG: /' orig.txt
    sed -i '' 's/^[^:]*: /PROG: /' mine.txt

    if diff orig.txt mine.txt > /dev/null 2>&1; then
        echo -e "[ ${GREEN}OK${NC} ] $label"
        ((PASS++))
    else
        echo -e "[ ${RED}FAIL${NC} ] $label"
        ((FAIL++))
        log "========================================"
        log "[ FAIL ] $label"
        log "  grep cmd : grep $grep_cmd"
        log "  my  cmd  : $MY_GREP $mygrep_cmd"
        log "--- expected (grep) ---"
        cat orig.txt >> "$LOG_FILE"
        log "--- got (s21_grep) ---"
        cat mine.txt >> "$LOG_FILE"
        log "--- diff (-u) ---"
        diff -u orig.txt mine.txt >> "$LOG_FILE"
        log ""
    fi
}

# ─────────────────────────────────────────────
# ТЕСТ 0: Без флагов
# ─────────────────────────────────────────────
echo ""
echo "--- [0] No flags ---"

run_test "no flags single file"         "$PATTERN $FILE"               "$PATTERN $FILE"
run_test "no flags two files"           "$PATTERN $FILE $FILE2"        "$PATTERN $FILE $FILE2"
run_test "no flags missing file"        "$PATTERN $MISSING_FILE"       "$PATTERN $MISSING_FILE"
run_test "no flags mixed (one missing)" "$PATTERN $FILE $MISSING_FILE" "$PATTERN $FILE $MISSING_FILE"

# ─────────────────────────────────────────────
# ТЕСТ 1: Все пары простых флагов — один файл
# ─────────────────────────────────────────────
echo ""
echo "--- [1] Pairs on single file ($FILE) ---"

for (( i=0; i<${#flags[@]}; i++ )); do
    for (( j=i+1; j<${#flags[@]}; j++ )); do
        f1=${flags[$i]}; f2=${flags[$j]}
        for combined in "-${f1}${f2}" "-${f2}${f1}"; do
            run_test "$combined $PATTERN $FILE" \
                     "$combined $PATTERN $FILE" \
                     "$combined $PATTERN $FILE"
        done
    done
done

# ─────────────────────────────────────────────
# ТЕСТ 2: Все пары простых флагов — два файла
# ─────────────────────────────────────────────
echo ""
echo "--- [2] Pairs on two files ($FILE $FILE2) ---"

for (( i=0; i<${#flags[@]}; i++ )); do
    for (( j=i+1; j<${#flags[@]}; j++ )); do
        f1=${flags[$i]}; f2=${flags[$j]}
        for combined in "-${f1}${f2}" "-${f2}${f1}"; do
            run_test "$combined $PATTERN $FILE $FILE2" \
                     "$combined $PATTERN $FILE $FILE2" \
                     "$combined $PATTERN $FILE $FILE2"
        done
    done
done

# ─────────────────────────────────────────────
# ТЕСТ 3: Все пары с несуществующим файлом
# ─────────────────────────────────────────────
echo ""
echo "--- [3] Pairs with missing file ($MISSING_FILE) ---"

for (( i=0; i<${#flags[@]}; i++ )); do
    for (( j=i+1; j<${#flags[@]}; j++ )); do
        f1=${flags[$i]}; f2=${flags[$j]}
        for combined in "-${f1}${f2}" "-${f2}${f1}"; do
            run_test "$combined $PATTERN $MISSING_FILE" \
                     "$combined $PATTERN $MISSING_FILE" \
                     "$combined $PATTERN $MISSING_FILE"
        done
    done
done

# ─────────────────────────────────────────────
# ТЕСТ 4: Пары с флагом -e
# ─────────────────────────────────────────────
echo ""
echo "--- [4] Pairs with -e flag (single file) ---"

for f in "${flags[@]}"; do
    for variant in "-e $PATTERN -$f" "-${f}e $PATTERN"; do
        run_test "$variant $FILE" "$variant $FILE" "$variant $FILE"
    done
done

echo ""
echo "--- [4b] Pairs with -e flag (two files) ---"

for f in "${flags[@]}"; do
    for variant in "-e $PATTERN -$f" "-${f}e $PATTERN"; do
        run_test "$variant $FILE $FILE2" "$variant $FILE $FILE2" "$variant $FILE $FILE2"
    done
done

# ─────────────────────────────────────────────
# ТЕСТ 5: Флаг -f со всеми остальными флагами
# ─────────────────────────────────────────────
echo ""
echo "--- [5] Pairs with -f flag (single file) ---"

for f in "${flags[@]}"; do
    for variant in "-f $PATTERN_FILE -$f" "-${f}f $PATTERN_FILE"; do
        run_test "$variant $FILE" "$variant $FILE" "$variant $FILE"
    done
done

echo ""
echo "--- [5b] Pairs with -f flag (two files) ---"

for f in "${flags[@]}"; do
    for variant in "-f $PATTERN_FILE -$f" "-${f}f $PATTERN_FILE"; do
        run_test "$variant $FILE $FILE2" "$variant $FILE $FILE2" "$variant $FILE $FILE2"
    done
done

echo ""
echo "--- [5c] -f with missing input file ---"

for f in "${flags[@]}"; do
    for variant in "-f $PATTERN_FILE -$f" "-${f}f $PATTERN_FILE"; do
        run_test "$variant $MISSING_FILE" "$variant $MISSING_FILE" "$variant $MISSING_FILE"
    done
done

# ─────────────────────────────────────────────
# ТЕСТ 6: -e и -f вместе
# ─────────────────────────────────────────────
echo ""
echo "--- [6] -e and -f combined ---"

run_test "-e $PATTERN -f $PATTERN_FILE $FILE" \
         "-e $PATTERN -f $PATTERN_FILE $FILE" \
         "-e $PATTERN -f $PATTERN_FILE $FILE"

run_test "-e $PATTERN -f $PATTERN_FILE $FILE $FILE2" \
         "-e $PATTERN -f $PATTERN_FILE $FILE $FILE2" \
         "-e $PATTERN -f $PATTERN_FILE $FILE $FILE2"

for f in "${flags[@]}"; do
    run_test "-e $PATTERN -f $PATTERN_FILE -$f $FILE" \
             "-e $PATTERN -f $PATTERN_FILE -$f $FILE" \
             "-e $PATTERN -f $PATTERN_FILE -$f $FILE"
done

# ─────────────────────────────────────────────
# ТЕСТ 7: Флаг -o со всеми остальными флагами
# -o выводит только совпавшую часть строки
# Не комбинируется с -c, -l (grep игнорирует -o рядом с ними)
# ─────────────────────────────────────────────
echo ""
echo "--- [7] Pairs with -o flag (single file) ---"

for f in "${flags[@]}"; do
    for combined in "-o$f" "-${f}o"; do
        run_test "$combined $PATTERN $FILE" \
                 "$combined $PATTERN $FILE" \
                 "$combined $PATTERN $FILE"
    done
done

echo ""
echo "--- [7b] Pairs with -o flag (two files) ---"

for f in "${flags[@]}"; do
    for combined in "-o$f" "-${f}o"; do
        run_test "$combined $PATTERN $FILE $FILE2" \
                 "$combined $PATTERN $FILE $FILE2" \
                 "$combined $PATTERN $FILE $FILE2"
    done
done

echo ""
echo "--- [7c] -o standalone ---"

run_test "-o single file"  "-o $PATTERN $FILE"        "-o $PATTERN $FILE"
run_test "-o two files"    "-o $PATTERN $FILE $FILE2" "-o $PATTERN $FILE $FILE2"

echo ""
echo "--- [7d] -o with -e ---"

for f in "${flags[@]}"; do
    run_test "-o -e $PATTERN -$f $FILE" \
             "-o -e $PATTERN -$f $FILE" \
             "-o -e $PATTERN -$f $FILE"
done

echo ""
echo "--- [7e] -o with -f ---"

for f in "${flags[@]}"; do
    run_test "-o -f $PATTERN_FILE -$f $FILE" \
             "-o -f $PATTERN_FILE -$f $FILE" \
             "-o -f $PATTERN_FILE -$f $FILE"
done

# ─────────────────────────────────────────────
# ТЕСТ 8: -h отдельно
# ─────────────────────────────────────────────
echo ""
echo "--- [8] -h standalone checks ---"

run_test "-h single file" "-h $PATTERN $FILE"        "-h $PATTERN $FILE"
run_test "-h two files"   "-h $PATTERN $FILE $FILE2" "-h $PATTERN $FILE $FILE2"

# ─────────────────────────────────────────────
# ТЕСТ 9: -s отдельно
# ─────────────────────────────────────────────
echo ""
echo "--- [9] -s standalone checks ---"

run_test "-s existing file"       "-s $PATTERN $FILE"               "-s $PATTERN $FILE"
run_test "-s missing file"        "-s $PATTERN $MISSING_FILE"       "-s $PATTERN $MISSING_FILE"
run_test "-s mixed (one missing)" "-s $PATTERN $FILE $MISSING_FILE" "-s $PATTERN $FILE $MISSING_FILE"

# ─────────────────────────────────────────────
# ИТОГ
# ─────────────────────────────────────────────
echo ""
echo "========================================"
echo -e "  RESULTS: ${GREEN}PASS: $PASS${NC} | ${RED}FAIL: $FAIL${NC}"
if [ $FAIL -gt 0 ]; then
    echo -e "  Log saved: ${RED}$LOG_FILE${NC}"
else
    echo "  All tests passed — no log written."
    rm -f "$LOG_FILE"
fi
echo "========================================"

rm -f orig.txt mine.txt test_complex.txt test_second.txt test_patterns.txt