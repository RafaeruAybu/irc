

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
 *      INVITE
 *      KICK
 *      MODE - нужно ли делать все флаги и вообще саму комманду?
 *
 ** Прочее
 *      PING
 *
 *
 *      --------------
 *
 *      Если смотреть по наборам команд, то получается 5
(PASS, NICK, USER)
todo: - (PRIVMSG, NOTICE) //AWAY не делаем, поэтому работают одинаково, кроме отправки ответа
todo: - (JOIN)
^^^Минимальный набор^^^
todo: - (OPER)
todo: - (KILL, ...)
 */


