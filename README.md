# ROS 2 Teleoperation for Omni-Wheel Robot

Управление мобильным роботом на omni-колёсах с клавиатуры с помощью ROS 2.

Принимает команды с клавиатуры, преобразует их в сообщения ROS 2 `geometry_msgs/Twist`, передаёт команды по UDP на NanoPi-AR и далее отправляет команды управления на OpenCM, который управляет DYNAMIXEL-приводами колёс.

## Архитектура

```text
Keyboard
   │
   ▼
teleop_twist_keyboard
   │  /cmd_vel (geometry_msgs/Twist)
   ▼
cmdvel_to_udp
   │  UDP
   ▼
NanoPi-AR
   │  Serial / USB commands: F, B, L, R, S
   ▼
OpenCM + STEM Board
   │  DYNAMIXEL bus
   ▼
Omni-wheel mobile robot
```

## ТРЕБУЕМОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ

### Программное обеспечение

- Ubuntu 22.04 или совместимая версия
- ROS 2 `<ROS_DISTRO>`
- Python 3
- Пакет `teleop_twist_keyboard`
- Пакет `aplied_ros` (репозиторий)

## Установка

### 1. Установить ROS 2

Установите ROS 2 согласно официальной инструкции для вашей версии Ubuntu и ROS 2.

### 2. Установить keyboard teleop

```bash
sudo apt update
sudo apt install ros-<ROS_DISTRO>-teleop-twist-keyboard
```

Например, для ROS 2 Humble:

```bash
sudo apt install ros-humble-teleop-twist-keyboard
```

### 3. Создать ROS 2 workspace

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
```

### 4. Клонировать репозиторий

```bash
git clone https://github.com/4tman/Aplied_Omniwheel.git

## Запуск
Подключите micro-USB к NanoPi-AR
Следом OpenCM к STEM Board

Терминал 1:
'''bash
ssh root@192.168.42.1 //Пароль 12345
python3 bridge.py
'''


Терминал 2:
```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

Терминал 3:
```bash
source ~/ros2_ws/install/setup.bash
ros2 run aplied_ros2 cmdvel_to_udp
```

## Управление

| Клавиша | Действие |
|---|---|
| `i` | Вперёд |
| `,` | Назад |
| `j` | Влево/поворот влево |
| `l` | Вправо/поворот вправо |
| 'k' | Полная остановка |


## Примечание

Перед управлением убедитесь, что DYNAMIXEL-приводы запитаны от внешнего источника питания.

## Структура проекта

```text
aplied_ros/
├── aplied_ros/
│   ├── __init__.py
│   └── cmdvel_to_udp.py
├── resource/
│   └── aplied_ros
├── package.xml
├── setup.py
└── setup.cfg
```

## Дальнейшее развитие
- Подключить камеру
- Добавить интерфейс оператора
- Добавить launch-файл для запуска всех компонентов одной командой
- Добавить параметры скорости через ROS 2 parameters и настройки dynamyxel
- Добавить обратную связь от OpenCM и DYNAMIXEL ??
- Добавить поддержку джойстика??
- Добавить видео демонстрации работы робота
