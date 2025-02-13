fetch('system_info.json')
    .then(response => response.json())
    .then(data => { 
        let infoDiv = document.getElementById('info');
        infoDiv.innerHTML = `
            <p class="info-item"><strong>Sistema Operacional:</strong> ${data.os}</p>
            <p class="info-item"><strong>Processador:</strong> ${data.cpu}</p>
            <p class="info-item"><strong>Temperatura da CPU:</strong> ${data.temperature}</p>
            <p class="info-item"><strong>Memória Total:</strong> ${data.memory.total}</p>
            <p class="info-item"><strong>Memória Usada:</strong> ${data.memory.used}</p>
            <p class="info-item"><strong>Memória Livre:</strong> ${data.memory.free}</p>
            <p class="info-item"><strong>Disco Total:</strong> ${data.disk.total}</p>
            <p class="info-item"><strong>Disco Usado:</strong> ${data.disk.used}</p>
            <p class="info-item"><strong>Disco Livre:</strong> ${data.disk.free}</p>
        `;
    })
    .catch(error => console.error('Erro ao carregar os dados:', error));
