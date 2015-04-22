node="/home/madks/StudyHelper"
sudo forever start -w -p $node/StudyHelper-WS-Rest -l $node/logs/ws.log -o $node/logs/ws-std.log -e $node/logs/ws-err.log -a --watchDirectory $node/StudyHelper-WS-Rest --sourceDir $node/StudyHelper-WS-Rest app.js
