      // ==========================================
// BANCOS DE DADOS LOCAIS (CONFIGURAÇÃO)
// ==========================================

// Dados para a análise postural e ergonômica (Pinos Interativos)
const hotspotData = {
  0: {
    title: "1. Acesso ao Escamoteador",
    desc: "A contenção e medicação de leitões exigem flexão da coluna lombar. Mantenha o corpo próximo à baia, flexione os joelhos e evite torções da coluna.",
    tip: "Orientação: Apoie um dos membros inferiores para distribuir o peso e alterne o lado frequentemente."
  },
  1: {
    title: "2. Arraçoamento da Matriz",
    desc: "O abastecimento manual de comedouros envolve movimentação de carga e inclinação do tronco. Mantenha a postura ereta, segure os baldes ou pás perto do centro do corpo e use a força das pernas.",
    tip: "Orientação: Evite girar o tronco enquanto levanta ou carrega o peso; mova os pés para mudar de direção."
  },
  2: {
    title: "3. Limpeza & Baias de Parto",
    desc: "A raspagem e higienização de pisos exigem posições estáticas prolongadas e esforço nos braços. Utilize cabos de vassoura ou rodos com comprimento adequado à sua altura para evitar trabalhar curvado.",
    tip: "Orientação: Alterne a empunhadura das ferramentas entre as mãos esquerda e direita para equilibrar o esforço muscular."
  }
};

// Dados para o Painel de Exercícios e Alongamentos
const exercisesDB = {
  costas: [
    {
      name: "Alongamento Lombar em Pé",
      how: "Fique em pé com os pés afastados na largura dos ombros. Apoie as mãos firmemente na região glútea/lombar e incline o tronco suavemente para trás, olhando para o teto. Segure a posição sem forçar.",
      dose: "3 séries de 20 segundos",
      goal: "Aliviar a compressão dos discos vertebrais após períodos curvados à frente.",
      care: "Mantenha os movimentos lentos. Não faça movimentos bruscos ou trancos."
    },
    {
      name: "Abraço de Joelhos (Descompressão)",
      how: "Sentado de forma estável, incline o tronco à frente deixando os braços caírem em direção aos pés, abraçando as pernas por trás se confortável. Relaxe o pescoço.",
      dose: "2 séries de 30 segundos",
      goal: "Alongar a musculatura paravertebral e reduzir a tensão acumulada.",
      care: "Retorne à posição ereta bem devagar para evitar tonturas."
    }
  ],
  ombros: [
    {
      name: "Alongamento de Peitoral e Ombros",
      how: "Entrelace os dedos das mãos atrás das costas. Estenda os braços para trás e para cima, empurrando o peito para a frente e aproximando as escápulas.",
      dose: "3 séries de 15 segundos",
      goal: "Corrigir a postura de ombros caídos gerada pelo manejo e transporte de cargas.",
      care: "Não curve a região lombar excessivamente enquanto puxa os braços."
    }
  ],
  pernas: [
    {
      name: "Alongamento de Quadríceps (Coxa)",
      how: "Apoie-se em uma parede ou estrutura firme. Flexione um dos joelhos para trás, segurando o pé com a mão do mesmo lado. Puxe o calcanhar em direção ao glúteo mantendo os joelhos alinhados.",
      dose: "2 séries de 20 segundos para cada lado",
      goal: "Reduzir o encurtamento muscular causado pela postura agachada constante.",
      care: "Mantenha o abdômen contraído e a postura ereta para não sobrecarregar o joelho."
    }
  ]
};


// ==========================================
// FUNÇÕES DE INTERAÇÃO DO SISTEMA
// ==========================================

function initDashboard() {
  const contentArea = document.getElementById('dash-content');
  const buttons = document.querySelectorAll('.dash-btn');

  function renderExercises(cat) {
    if (!contentArea) return;
    contentArea.innerHTML = '';
    const items = exercisesDB[cat] || [];

    items.forEach(ex => {
      const itemDiv = document.createElement('div');
      itemDiv.className = 'dash-exercise-item';
      itemDiv.innerHTML = `
        <h3>${ex.name}</h3>
        <p><strong>Execução:</strong> ${ex.how}</p>
        <div class="dash-meta-grid">
          <div><strong>Dose Sugerida:</strong> ${ex.dose}</div>
          <div><strong>Objetivo Principal:</strong> ${ex.goal}</div>
          <div class="dash-meta-care"><strong>⚠ Ponto de Atenção:</strong> ${ex.care}</div>
        </div>
      `;
      contentArea.appendChild(itemDiv);
    });

    // MELHORIA: Faz o painel rolar de volta para o topo ao trocar de categoria
    contentArea.scrollTop = 0;
  }

  buttons.forEach(btn => {
    btn.addEventListener('click', () => {
      buttons.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      renderExercises(btn.getAttribute('data-filter'));
    });
  });

  // Inicializa exibindo os exercícios de costas por padrão
  renderExercises('costas'); 
}


function initTimer() {
  let timeLeft = 180;
  let timerInterval = null;

  const countEl = document.getElementById('timer-text');
  const btnToggle = document.getElementById('btn-timer-start');
  const btnReset = document.getElementById('btn-timer-reset');

  function update() {
    const mins = Math.floor(timeLeft / 60);
    const secs = timeLeft % 60;
    if (countEl) countEl.textContent = `${String(mins).padStart(2, '0')}:${String(secs).padStart(2, '0')}`;
  }

  if (btnToggle) {
    btnToggle.addEventListener('click', () => {
      if (timerInterval) {
        clearInterval(timerInterval);
        timerInterval = null;
        btnToggle.textContent = 'Continuar';
        return;
      }
      btnToggle.textContent = 'Pausar';
      timerInterval = setInterval(() => {
        if (timeLeft > 0) {
          timeLeft--;
          update();
        } else {
          clearInterval(timerInterval);
          timerInterval = null;
          btnToggle.textContent = 'Concluído';
          alert('Pausa ativa concluída com sucesso!');
        }
      }, 1000);
    });
  }

  if (btnReset) {
    btnReset.addEventListener('click', () => {
      if (timerInterval) clearInterval(timerInterval);
      timerInterval = null;
      timeLeft = 180;
      update();
      if (btnToggle) btnToggle.textContent = 'Iniciar';
      
      // MELHORIA: Busca os elementos atualizados na árvore na hora de resetar
      const currentCheckboxes = document.querySelectorAll('.routine-checklist input[type="checkbox"]');
      currentCheckboxes.forEach(c => c.checked = false);
    });
  }
  update();
}


function initHotspots() {
  const titleEl = document.getElementById('hotspot-title');
  const descEl = document.getElementById('hotspot-desc');
  const tipEl = document.getElementById('hotspot-tip');
  
  const allButtons = [
    ...document.querySelectorAll('.btn-hotspot'),
    ...document.querySelectorAll('.hotspot-pin')
  ];

  allButtons.forEach(btn => {
    btn.addEventListener('click', () => {

      allButtons.forEach(b => b.classList.remove('active'));
      
      const pointId = btn.getAttribute('data-point');
      const data = hotspotData[pointId];

      document.querySelectorAll(`[data-point="${pointId}"]`).forEach(b => b.classList.add('active'));

      if (titleEl && data) titleEl.textContent = data.title;
      if (descEl && data) descEl.textContent = data.desc;
      if (tipEl && data) tipEl.textContent = data.tip;
    });
  });
}


// Executa todas as funções assim que a página terminar de carregar inteiramente
window.onload = function() {
  try { if (window.lucide) window.lucide.createIcons(); } catch(e){}
  try { initDashboard(); } catch(e){}
  try { initTimer(); } catch(e){}
  try { initHotspots(); } catch(e){}
};
