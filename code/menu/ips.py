import time
import threading
from abc import abstractmethod, ABC

ips = [""]*16;
ips_flash_success=False;

buttonInput = [];

def main():
    global ips_flash_success;
    printThread = threading.Thread(target = ipsPrint);
    printThread.start();
    buttonThread = threading.Thread(target = getButton);
    buttonThread.start();
    
    while True:
        while ips_flash_success:pass;
        menu.print();
        ips_flash_success = True;
        if(len(buttonInput) > 0):
            buttonPressed:int = buttonInput.pop(0);
            menu.press(buttonPressed);

def getButton():
    while True:
        if len(buttonInput) == 0:
            for i in input(""):
                if i not in "1234":
                    continue;
                buttonInput.append(int(i)-1);

def ipsPrint():
    global ips_flash_success;
    while True:
        while not ips_flash_success:pass;
        pr = "";
        pr += "\n"*4;
        for i in range(16):
            pr += "\n" + ips[i][:24];
        print(pr);
        ips_flash_success = False;
        time.sleep(0.1);

def ips_clear():
    global ips;
    ips = [""]*16;

def ips_show_string(x:int, y:int, string:str):
    cx = x//10;
    cy = y//20;
    line = ips[cy];
    line = line[0:cx] + " "*(cx-len(line) if cx-len(line) > 0 else 0) + string + line[cx+len(string):];
    ips[cy] = line;

def ips_show_int(x:int, y:int, i:int, num:int):
    ips_show_string(x, y, str(i)[:num]);

def ips_show_float(x:int, y:int, i:float, num:int):
    ips_show_string(x, y, str(i)[:num]);

class PageKey():
    name:str;
    type:str;
    value:"PageValue";
    opened:int = 0;
    open:bool = False;
    def __init__(this, name:str, type:str, value:"PageValue") -> None:
        this.name = name;
        this.type = type;
        this.value = value;
    def print(this) -> None:
        if this.type == "page":
            ips_clear();
            ips_show_string(0, 0, this.name);
        this.value.print(this.opened, this.open);
    def press(this, button:int) -> None:
        this.opened, this.open = this.value.press(button, this.opened, this.open);

class PageValue(ABC):
    @abstractmethod
    def print(this, opened:int, open:bool) -> None:
        ...
    @abstractmethod
    def press(this, button:int, opened:int, open:bool) -> tuple[int, bool]:
        ...
    
class PagePageValue(PageValue):
    pageKey:list;
    def __init__(this, pageKey:list) -> None:
        this.pageKey = pageKey;
    def print(this, opened:int, open:bool) -> None:
        if(opened >= 0):
            if open and this.pageKey[opened].type == "page":
                this.pageKey[opened].print();
                return;
            else:
                ips_show_string(230, 20*(opened+1), "<" if open else "-");
        for i in range(len(this.pageKey)):
            ips_show_string(10, 20*(i+1), this.pageKey[i].name);
            if this.pageKey[i].type != "page":
                this.pageKey[i].opened = i;
                this.pageKey[i].print();
    def press(this, button:int, opened:int, open:bool) -> tuple[int, bool]:
        if button == 0:
            pageKey = this.pageKey[opened];
            if pageKey.type != "page" or not pageKey.open:   
                return opened, False;
        if open:
            this.pageKey[opened].press(button);
            return opened, True;
        else:
            if button == 1:
                opened-=1;
                if opened < 0:
                    opened = len(this.pageKey)-1;
                return opened, False;
            if button == 2:
                opened+=1
                if opened >= len(this.pageKey):
                    opened = 0;
                return opened, False;
            if button == 3:
                return opened, True;
            

class IntPageValue(PageValue):
    intList:list;
    max:int;
    min:int;
    step:int;
    def __init__(this, intList:list, max:int, min:int, step:int) -> None:
        this.intList = intList;
        this.max = max;
        this.min = min;
        this.step = step;
    def print(this, opened:int, open:bool) -> None:
        ips_show_int(180, 20*(opened+1), this.intList[0], 5);
    def press(this, button:int, opened:int, open:bool) -> tuple[int, bool]:
        if button == 1:
            this.intList[0] += this.step;
            if this.intList[0] > this.max:
                this.intList[0] = this.max
            return opened, False;
        if button == 2:
            this.intList[0] -= this.step;
            if this.intList[0] < this.min:
                this.intList[0] = this.min
            return opened, False;
        if button == 3:
            return opened, False;

class FloatPageValue(PageValue):
    floatList:list;
    max:float;
    min:float;
    step:float;
    def __init__(this, floatList:list, max:float, min:float, step:float) -> None:
        this.floatList = floatList;
        this.max = max;
        this.min = min;
        this.step = step;
    def print(this, opened:int, open:bool) -> None:
        ips_show_float(180, 20*(opened+1), this.floatList[0], 5);
    def press(this, button:int, opened:int, open:bool) -> tuple[int, bool]:
        if button == 1:
            this.floatList[0] += this.step;
            if this.floatList[0] > this.max:
                this.floatList[0] = this.max
            return opened, False;
        if button == 2:
            this.floatList[0] -= this.step;
            if this.floatList[0] < this.min:
                this.floatList[0] = this.min
            return opened, False;
        if button == 3:
            return opened, False;

class BooleanPageValue(PageValue):
    boolList:list;
    def __init__(this, boolList:list) -> None:
        this.boolList = boolList;
    def print(this, opened:int, open:bool) -> None:
        ips_show_string(180, 20*(opened+1), "true" if this.boolList[0] else "false");
    def press(this, button:int, opened:int, open:bool) -> tuple[int, bool]:
        if button == 1:
            this.boolList[0] = True;
            return opened, False;
        if button == 2:
            this.boolList[0] = False;
            return opened, False;
        if button == 3:
            return opened, False;

class FunctionPageValue(PageValue):
    upFunc:"function";
    downFunc:"function";
    def __init__(this, upFunc:"function", downFunc:"function") -> None:
        this.upFunc = upFunc;
        this.downFunc = downFunc
    def print(this, opened:int, open:bool) -> None:
        pass;
    def press(this, button:int, opened:int, open:bool) -> tuple[int, bool]:
        if button == 1:
            this.upFunc();
            return opened, False;
        if button == 2:
            this.downFunc();
            return opened, False;
        if button == 3:
            return opened, False;

start = [False];
def stop():
    start[0] = False;

menu:PageKey = PageKey(
    "main", "page", PagePageValue(
        [
            PageKey(
                "PID", "page", PagePageValue(
                    [
                        PageKey(
                            "motor_l", "page", PagePageValue(
                                [
                                    PageKey(
                                        "Kp", "float", FloatPageValue(
                                            [12.0], 100., 0., 0.1
                                        )
                                    ),
                                    PageKey(
                                        "Ki", "float", FloatPageValue(
                                            [0.4], 100., 0., 0.1
                                        )
                                    ),
                                    PageKey(
                                        "Kd", "float", FloatPageValue(
                                            [1.4], 100., 0., 0.1
                                        )
                                    )
                                ]
                            )
                        ),
                        PageKey(
                            "motor_r", "page", PagePageValue(
                                [
                                    PageKey(
                                        "Kp", "float", FloatPageValue(
                                            [12.0], 100., 0., 0.1
                                        )
                                    ),
                                    PageKey(
                                        "Ki", "float", FloatPageValue(
                                            [0.4], 100., 0., 0.1
                                        )
                                    ),
                                    PageKey(
                                        "Kd", "float", FloatPageValue(
                                            [1.4], 100., 0., 0.1
                                        )
                                    )
                                ]
                            )
                        ),
                        PageKey(
                            "turn", "page", PagePageValue(
                                [
                                    PageKey(
                                        "Kp", "float", FloatPageValue(
                                            [1.0], 100., 0., 0.1
                                        )
                                    ),
                                    PageKey(
                                        "Ki", "float", FloatPageValue(
                                            [0.2], 100., 0., 0.1
                                        )
                                    ),
                                    PageKey(
                                        "Kd", "float", FloatPageValue(
                                            [0.1], 100., 0., 0.1
                                        )
                                    )
                                ]
                            )
                        )
                    ]
                )
            ),
            PageKey(
                "start", "bool", BooleanPageValue(
                    start
                )
            ),
            PageKey(
                "stop", "func", FunctionPageValue(
                    stop, stop
                )
            )
        ]
    )
);

if __name__ == "__main__":
    main();