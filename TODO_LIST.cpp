

//TODO: Выяснить, должен ли в Adium быть список юзеров на сервере

/*** Команды
 ** Регистрация
 *      PASS
 *      NICK
 *      USER
 *      OPER
 *      QUIT
 *
 ** Сообщения
 *      PRIVMSG <receiver>{,<receiver>} <text to be sent>
 *      NOTICE <nickname> <text> (не нужен ответ)
 *
 ** Каналы
 *      JOIN
 *      PART - покинуть канал
 *      INVITE
 *      KICK
 *      MODE - нужно ли делать все флаги и вообще саму комманду?
 *
 ** Прочее
 *      PING
 *      LIST
 *
 *      --------------
 *
 *      Если смотреть по наборам команд, то получается 5
(PASS, NICK, USER)
(PRIVMSG, NOTICE) //AWAY не делаем, поэтому работают одинаково, кроме отправки ответа - сделать отправку в канал
 - (JOIN) - разобраться с сегой в Adium при рассылке о коннекте нового юзера
Удаление юзера из Channel при выходе с сервера
Ответ на JOIN ":IRCat 353 oper = #chan :@dduck oper" - список юзеров
^^^Минимальный набор^^^
todo: (LIST)
todo: (PART)
(OPER) - <user> <password> - ставится флаг оператора сервера
todo: (KICK) только чопер, кикнуть с канала
todo: - (KILL, ...)

eraseUserFromChannel - (отправка сообщения всем в канале, что юзер вышел)
    *НЕ нужно - сообщение о выходе юзера отправляется не в канал, а всем юзерам на сервере(делать не будем, ушел и ушел)

 https://www.lissyara.su/doc/rfc/rfc1459/
 */


