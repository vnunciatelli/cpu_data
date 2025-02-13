require 'rbconfig'
require 'sys/proctable'
require 'json'

# Detecta o sistema operacional
def os
  @os ||= case RbConfig::CONFIG['host_os']
          when /mswin|mingw|cygwin/ then 'windows'
          when /darwin/ then 'macos'
          when /linux/ then 'linux'
          else 'unknown'
          end
end

# Obtém informações do processador
def cpu_info
  case os
  when 'windows'
    `wmic cpu get Name`.split("\n")[1].strip
  when 'macos', 'linux'
    `cat /proc/cpuinfo | grep 'model name' | uniq`.split(':').last.strip rescue 'N/A'
  else
    'N/A'
  end
end

# Obtém a temperatura da CPU
def cpu_temperature
  case os
  when 'linux'
    `sensors | grep 'Package id 0' | awk '{print $4}'`.strip rescue 'N/A'
  when 'macos'
    `osx-cpu-temp`.strip rescue 'N/A'
  else
    'N/A'
  end
end

# Obtém informações de memória
def memory_info
  case os
  when 'windows'
    `wmic OS get TotalVisibleMemorySize,FreePhysicalMemory /Value`.strip.split("\n")
  when 'linux', 'macos'
    {
      total: `free -m | grep Mem | awk '{print $2}'`.strip + ' MB',
      used: `free -m | grep Mem | awk '{print $3}'`.strip + ' MB',
      free: `free -m | grep Mem | awk '{print $4}'`.strip + ' MB'
    }
  else
    'N/A'
  end
end

# Obtém informações do disco
def disk_info
  case os
  when 'windows'
    `wmic logicaldisk get Size,FreeSpace`.strip.split("\n")
  when 'linux', 'macos'
    {
      total: `df -h / | awk 'NR==2 {print $2}'`.strip,
      used: `df -h / | awk 'NR==2 {print $3}'`.strip,
      free: `df -h / | awk 'NR==2 {print $4}'`.strip
    }
  else
    'N/A'
  end
end

# Exibir as informações
info = {
  os: os,
  cpu: cpu_info,
  temperature: cpu_temperature,
  memory: memory_info,
  disk: disk_info
}

puts JSON.pretty_generate(info)
