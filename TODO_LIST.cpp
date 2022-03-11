

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
todo: - (PRIVMSG, NOTICE) //AWAY не делаем, поэтому работают одинаково, кроме отправки ответа - сделать отправку в канал
todo: - (JOIN)
^^^Минимальный набор^^^
todo: (LIST)
todo: (PART)
todo: - (OPER)
todo: (KICK) только чопер, кикнуть с канала
todo: - (KILL, ...)

 https://www.lissyara.su/doc/rfc/rfc1459/
 */


