# Установка ПО для Mac OS X

1. Установка пакетного менеджера `brew` (http://brew.sh/). Для установки нужно в терминал ввести команду `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
2. Установка `opencv` (http://jjyap.wordpress.com/2014/05/24/installing-opencv-2-4-9-on-mac-osx-with-python-support/).
2.1 Добавление репозитория в `brew`, где хранится `opencv` с помощью команды: `brew tap homebrew/science`
2.2 Установка opencv: `brew install opencv`
3. Установка `qt` версии `4.8.6` (http://qt-project.org/downloads#qt-lib)
3.1 Скачиваем `Qt libraries 4.8.6 for Mac (185 MB)` (http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-opensource-mac-4.8.6-1.dmg)
3.2 Кликаем 2 раза по скачанному файлу (монтируем образ) и устанавливаем с помощью стандартного системного пакетного менеджера.
4. Установка `qt creator` (http://qt-project.org/downloads#qt-lib)
4.1 Аналогично предыдущему шагу скачиваем и устанавливаем `Qt Creator 3.2.1 for Mac (70 MB)` (http://download.qt-project.org/official_releases/qtcreator/3.2/3.2.1/qt-creator-opensource-mac-x86_64-3.2.1.dmg)
5. Скачиваем проект из `bitbucket'a` с помощью `git` или с помощью простого скачивания проекта в виде архива. (будет описано позднее)
6. Настройка проекта (будет описано позднее)

## Особенности отладки под MAC OS X 

В силу проблем `QT` с MAC OS X и известных проблем с отладкой http://blog.qt.digia.com/blog/2013/11/12/qt-creator-and-lldb/ , они поддерживают нормально (по их словам) `lldb  300.22`, что соответствует `XCode 5 Command line tools`.  Поэтому чтобы это вылечить для `XCode 6` и `lldb 320.x`, нужно скачать патчи (можно только второй взять) https://codereview.qt-project.org/#/c/95516/ ( в соответствии с https://bugreports.qt-project.org/browse/QTCREATORBUG-13077?page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel ).

### Что делать со скачанным(и) патчем (патчами) ?

1. Достаточно взять только один из них и из директории `<downloaded directory>/share/qtcreator/debugger/ ` взять и скопировать файлик `lldbbridge.py`  ( например, вот так можно взять в память `cat lldbbridge.py | pbcopy`) .
2. После этого нужно найти `QTCreator.app` и правой кнопкой выбрать `Show package contents`. 
3. После этого зайти в папку `Resources/debugger` и найти файл с аналогичным названием. ( у меня получился путь `/Applications/Qt Creator.app/Contents/Resources/debugger`)
4. Заменить содержимое этого файла содержимым из патча. Если вы скопировали это в буфер обмена, как и я, достаточно открыть этот файлик в редакторе и вставить.
5. Перезапустить qt creator. 
6. Voilà!