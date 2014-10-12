# Особенности отладки под MAC OS X 

В силу проблем `QT` с MAC OS X и известных проблем с отладкой http://blog.qt.digia.com/blog/2013/11/12/qt-creator-and-lldb/ , они поддерживают нормально (по их словам) `lldb  300.22`, что соответствует `XCode 5 Command line tools`.  Поэтому чтобы это вылечить для `XCode 6` и `lldb 320.x`, нужно скачать патчи (можно только второй взять) https://codereview.qt-project.org/#/c/95516/ ( в соответствии с https://bugreports.qt-project.org/browse/QTCREATORBUG-13077?page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel ).

## Что делать со скачанным(и) патчем (патчами) ?

1. Достаточно взять только один из них и из директории `<downloaded directory>/share/qtcreator/debugger/ ` взять и скопировать файлик `lldbbridge.py`  ( например, вот так можно взять в память `cat lldbbridge.py | pbcopy`) .
2. После этого нужно найти `QTCreator.app` и правой кнопкой выбрать `Show package contents`. 
3. После этого зайти в папку `Resources/debugger` и найти файл с аналогичным названием. ( у меня получился путь `/Applications/Qt Creator.app/Contents/Resources/debugger`)
4. Заменить содержимое этого файла содержимым из патча. Если вы скопировали это в буфер обмена, как и я, достаточно открыть этот файлик в редакторе и вставить.
5. Перезапустить qt creator. 
6. Voilà!