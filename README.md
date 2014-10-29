# Установка ПО для Windows 7+

1. Установка `Visual Studio 2010` (можно поставить `VS2010 Express`)
2. Устанавливаем`opencv 2.4.9` http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.4.9/  и распаковываем, например, в `"C:\opencv"`.  
2.1.  После распаковки `opencv` нужно указать переменную окружения `OPENCV_DIR="C:\opencv\build\x86\vc10"`.  
3. Устанавливаем `qt 4.8.6` http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-opensource-windows-x86-vs2010-4.8.6.exe  
4. Устанавливаем `qt creator` http://download.qt-project.org/official_releases/qtcreator/3.2/3.2.1/qt-creator-opensource-windows-x86-3.2.1.exe  
5. Настройка `qt creator` будет описана в отдельном разделе

# Установка ПО для Mac OS X

0. Установка `XCode` и `Xcode command line tools`. 

* В терминале можно проверить, что установился `Xcode` командой 
```shell
xcode-select -p
```

* После установки `Xcode` нужно запросить установку `Xcode command line tools`, выполнив следующую команду в терминале:
```shell
 xcode-select --install
```

1. Установка пакетного менеджера `brew` (http://brew.sh/). Для установки нужно в терминал ввести команду 
```shell
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
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

---
#Дополнительно

## Настройка среды `Qt Creator`

Ниже будет описана настройка среды `Qt Creator 3.2.1` в `Windows 7+`. Аналогично настраивается среда и в `Mac OS X` (детали, касающиеся отладки, смотреть в отдельном разделе).

1. Указываем путь к `qmake.exe`, соответствующем `QT 4.8.6`:

    ![Qmake-Version](http://dl2.joxi.net/drive/0003/2969/248729/141105/b05b2c40f6.jpg "Указываем нужную версию Qt")  

2. Устанавливаем отладчик.  Для этого сначала нужно скачать и установить [Windows Kits 8.x](http://msdn.microsoft.com/en-us/windows/hardware/hh852365).

3. После установки в окне `Debuggers` должны появиться автоматически найденные отладчики
    ![Debuggers](http://dl2.joxi.net/drive/0003/2969/248729/141105/aad74f74de.jpg "Окно настроек Debuggers")  

4. "Собираем" `Qt Kit`, соответствующий текущей конфигурации. Для этого  клонируем конфигурацию `Desktop` и создаём новую, скажем, `QT 4.8.6`. 

    ![Qt-Kit](http://dl2.joxi.net/drive/0003/2969/248729/141105/0caf5ef67a.jpg "Окно настроек Kits")  

5. Далее выбираем версию отладчика `x86`, версию `qt` и указываем компилятор `Microsoft Visual C++ Compiler (x86)`

## Особенности отладки под MAC OS X 

В силу проблем `QT` с MAC OS X и известных проблем с отладкой http://blog.qt.digia.com/blog/2013/11/12/qt-creator-and-lldb/ , они поддерживают нормально (по их словам) `lldb  300.22`, что соответствует `XCode 5 Command line tools`.  Поэтому чтобы это вылечить для `XCode 6` и `lldb 320.x`, нужно скачать патчи (можно только второй взять) https://codereview.qt-project.org/#/c/95516/ ( в соответствии с https://bugreports.qt-project.org/browse/QTCREATORBUG-13077?page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel ).

### Что делать со скачанным(и) патчем (патчами) ?

1. Достаточно взять только один из них и из директории `<downloaded directory>/share/qtcreator/debugger/ ` взять и скопировать файлик `lldbbridge.py`  ( например, вот так можно взять в память `cat lldbbridge.py | pbcopy`) .
2. После этого нужно найти `QTCreator.app` и правой кнопкой выбрать `Show package contents`. 
3. После этого зайти в папку `Resources/debugger` и найти файл с аналогичным названием. ( у меня получился путь `/Applications/Qt Creator.app/Contents/Resources/debugger`)
4. Заменить содержимое этого файла содержимым из патча. Если вы скопировали это в буфер обмена, как и я, достаточно открыть этот файлик в редакторе и вставить.
5. Перезапустить qt creator. 
6. Voilà!