# Тест только для unset

# ##############################
unset HOME
echo $HOME


# Тут будет уже различия между оболочками ############################################################
unset
unset \?
unset dfhfdghgfjhfgt
unset вкенвкпнвкрварварва
unset HOME PATH drghdfghd
echo $PATH

 # Ошибки в синтаксисе ##############################
echo 'Тестируем: ошибки в синтаксисе'
./unset/01.sh;echo $?
./unset/02.sh;echo $?

# ##############################
echo 'Это должно всегда отображаться'
