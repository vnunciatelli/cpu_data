package main

import (
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"strings"

	"github.com/shirou/gopsutil/cpu"
	"github.com/shirou/gopsutil/mem"
	"github.com/shirou/gopsutil/disk"
)

func getOS() string {
	// Verifica o sistema operacional
	return os.Getenv("GOOS")
}

func getCPUInfo() string {
	if getOS() == "windows" {
		cmd := exec.Command("wmic", "cpu", "get", "caption")
		output, err := cmd.Output()
		if err != nil {
			return "N/A"
		}
		return strings.TrimSpace(string(output))
	} else if getOS() == "linux" {
		cmd := exec.Command("sh", "-c", "cat /proc/cpuinfo | grep 'model name' | uniq")
		output, err := cmd.Output()
		if err != nil {
			return "N/A"
		}
		return strings.TrimSpace(strings.Split(string(output), ":")[1])
	} else if getOS() == "darwin" {
		cmd := exec.Command("sysctl", "-n", "machdep.cpu.brand_string")
		output, err := cmd.Output()
		if err != nil {
			return "N/A"
		}
		return strings.TrimSpace(string(output))
	}
	return "N/A"
}

func getCPUTemperature() string {
	if getOS() == "linux" {
		cmd := exec.Command("sh", "-c", "sensors | grep 'Package id 0' | awk '{print $4}'")
		output, err := cmd.Output()
		if err != nil {
			return "N/A"
		}
		return strings.TrimSpace(string(output))
	} else if getOS() == "darwin" {
		cmd := exec.Command("osx-cpu-temp")
		output, err := cmd.Output()
		if err != nil {
			return "N/A"
		}
		return strings.TrimSpace(string(output))
	}
	return "N/A"
}

func getMemoryInfo() map[string]string {
	v, _ := mem.VirtualMemory()
	memoryInfo := make(map[string]string)
	memoryInfo["total"] = fmt.Sprintf("%d MB", v.Total/(1024*1024))
	memoryInfo["used"] = fmt.Sprintf("%d MB", v.Used/(1024*1024))
	memoryInfo["free"] = fmt.Sprintf("%d MB", v.Available/(1024*1024))
	return memoryInfo
}

func getDiskInfo() map[string]string {
	diskUsage, _ := disk.Usage("/")
	diskInfo := make(map[string]string)
	diskInfo["total"] = fmt.Sprintf("%d GB", diskUsage.Total/(1024*1024*1024))
	diskInfo["used"] = fmt.Sprintf("%d GB", diskUsage.Used/(1024*1024*1024))
	diskInfo["free"] = fmt.Sprintf("%d GB", diskUsage.Free/(1024*1024*1024))
	return diskInfo
}

func main() {
	data := map[string]interface{}{
		"os":          getOS(),
		"cpu":         getCPUInfo(),
		"temperature": getCPUTemperature(),
		"memory":      getMemoryInfo(),
		"disk":        getDiskInfo(),
	}

	// Salva em arquivo JSON
	file, err := os.Create("system_info.json")
	if err != nil {
		fmt.Println("Erro ao criar arquivo:", err)
		return
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "    ")
	err = encoder.Encode(data)
	if err != nil {
		fmt.Println("Erro ao salvar os dados no arquivo:", err)
		return
	}

	fmt.Println("Dados salvos em system_info.json")
}
