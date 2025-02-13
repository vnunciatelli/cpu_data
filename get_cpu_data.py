import platform
import psutil
import json
import subprocess

def get_os():
    return platform.system().lower()

def get_cpu_info():
    if get_os() == 'windows':
        return platform.processor()
    elif get_os() == 'linux':
        return subprocess.getoutput("cat /proc/cpuinfo | grep 'model name' | uniq").split(':')[-1].strip()
    elif get_os() == 'darwin':
        return subprocess.getoutput("sysctl -n machdep.cpu.brand_string").strip()
    return 'N/A'

def get_cpu_temperature():
    if get_os() == 'linux':
        try:
            return subprocess.getoutput("sensors | grep 'Package id 0' | awk '{print $4}'").strip()
        except:
            return 'N/A'
    elif get_os() == 'darwin':
        try:
            return subprocess.getoutput("osx-cpu-temp").strip()
        except:
            return 'N/A'
    return 'N/A'

def get_memory_info():
    mem = psutil.virtual_memory()
    return {
        'total': f'{mem.total // (1024 * 1024)} MB',
        'used': f'{mem.used // (1024 * 1024)} MB',
        'free': f'{mem.available // (1024 * 1024)} MB'
    }

def get_disk_info():
    disk = psutil.disk_usage('/')
    return {
        'total': f'{disk.total // (1024 * 1024 * 1024)} GB',
        'used': f'{disk.used // (1024 * 1024 * 1024)} GB',
        'free': f'{disk.free // (1024 * 1024 * 1024)} GB'
    }

data = {
    'os': get_os(),
    'cpu': get_cpu_info(),
    'temperature': get_cpu_temperature(),
    'memory': get_memory_info(),
    'disk': get_disk_info()
}

with open('system_info.json', 'w') as json_file:
    json.dump(data, json_file, indent=4)

print("Dados salvos em system_info.json")
