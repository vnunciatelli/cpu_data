<?php

function getOS() {
    return PHP_OS;
}

function getCPUInfo() {
    $os = getOS();
    if ($os === 'WINNT') {
        // Comando para pegar a CPU no Windows
        $output = shell_exec('wmic cpu get caption');
        return trim($output);
    } elseif ($os === 'Linux') {
        // Comando para pegar a CPU no Linux
        $output = shell_exec("cat /proc/cpuinfo | grep 'model name' | uniq");
        return trim(explode(":", $output)[1]);
    } elseif ($os === 'Darwin') {
        // Comando para pegar a CPU no macOS
        $output = shell_exec("sysctl -n machdep.cpu.brand_string");
        return trim($output);
    }
    return 'N/A';
}

function getCPUTemperature() {
    $os = getOS();
    if ($os === 'Linux') {
        // Comando para pegar a temperatura da CPU no Linux
        $output = shell_exec("sensors | grep 'Package id 0' | awk '{print $4}'");
        return trim($output);
    } elseif ($os === 'Darwin') {
        // Comando para pegar a temperatura da CPU no macOS
        $output = shell_exec("osx-cpu-temp");
        return trim($output);
    }
    return 'N/A';
}

function getMemoryInfo() {
    $memoryInfo = [];
    $os = getOS();

    if ($os === 'Linux') {
        // Comando para pegar a memória no Linux
        $output = shell_exec("free -m | grep Mem");
        $data = preg_split('/\s+/', $output);
        $memoryInfo['total'] = $data[1] . ' MB';
        $memoryInfo['used'] = $data[2] . ' MB';
        $memoryInfo['free'] = $data[3] . ' MB';
    } elseif ($os === 'Darwin') {
        // Comando para pegar a memória no macOS
        $output = shell_exec("sysctl -n hw.memsize");
        $memoryInfo['total'] = $output . ' bytes'; // Valor em bytes, você pode converter para MB ou GB
        // Não há comando direto para memória usada e livre no macOS, precisa de abordagem diferente
        $memoryInfo['used'] = 'N/A';
        $memoryInfo['free'] = 'N/A';
    }

    return $memoryInfo;
}

function getDiskInfo() {
    $diskInfo = [];
    $output = shell_exec("df -h /");
    $data = preg_split('/\s+/', $output);
    $diskInfo['total'] = $data[1];
    $diskInfo['used'] = $data[2];
    $diskInfo['free'] = $data[3];
    
    return $diskInfo;
}

$data = [
    'os' => getOS(),
    'cpu' => getCPUInfo(),
    'temperature' => getCPUTemperature(),
    'memory' => getMemoryInfo(),
    'disk' => getDiskInfo()
];

// Salvando as informações em um arquivo JSON
file_put_contents('system_info.json', json_encode($data, JSON_PRETTY_PRINT));

echo "Dados salvos em system_info.json\n";
?>
