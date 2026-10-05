 import { exercisesDB, hotspotData } from "./dados.js"; // <- CORREÇÃO: Adicionada a importação de hotspotData e a extensão .js

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
          <div><strong>Dose Sugerida:</strong> <br>${ex.dose}</div>
          <div><strong>Objetivo Principal:</strong> <br>${ex.goal}</div>
          <div class="dash-meta-care"><strong>⚠ Ponto de Atenção:</strong> <br>${ex.care}</div>
        </div>
      `;
      contentArea.appendChild(itemDiv);
    });
  }

  buttons.forEach(btn => {
    btn.addEventListener('click', () => {
      buttons.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      renderExercises(btn.getAttribute('data-filter'));
    });
  });

  renderExercises('costas'); 
}

function initTimer() {
  let timeLeft = 180;
  let timerInterval = null;

  const countEl = document.getElementById('timer-text');
  const btnToggle = document.getElementById('btn-timer-start');
  const btnReset = document.getElementById('btn-timer-reset');
  const checkboxes = document.querySelectorAll('.routine-checklist input[type="checkbox"]');

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
      checkboxes.forEach(c => c.checked = false);
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

  // Adiciona a verificação caso os dados não existam para evitar erros no console
  if (!hotspotData) {
      console.error("Dados de Hotspot não encontrados. Verifique a exportação em dados.js");
      return; 
  }

  allButtons.forEach(btn => {
    btn.addEventListener('click', () => {
      allButtons.forEach(b => b.classList.remove('active'));
      
      const pointId = btn.getAttribute('data-point');
      const data = hotspotData[pointId];

      if (data) {
          document.querySelectorAll(`[data-point="${pointId}"]`).forEach(b => b.classList.add('active'));
          if (titleEl) titleEl.textContent = data.title;
          if (descEl) descEl.textContent = data.desc;
          if (tipEl) tipEl.textContent = data.tip;
      }
    });
  });
}

window.onload = function() {
  try { if (window.lucide) window.lucide.createIcons(); } catch(e){}
  try { initDashboard(); } catch(e){}
  try { initTimer(); } catch(e){}
  try { initHotspots(); } catch(e){}
};