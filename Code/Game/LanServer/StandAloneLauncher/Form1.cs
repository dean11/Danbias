using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using System.Threading;
using System.Timers;

namespace StandAloneLauncher
{
    public partial class Form1 : Form
    {
        System.Windows.Interop.StandaloneGameServerCLI gameServer;
        bool serverIsRunning = false;

        public Form1()
        {
            InitializeComponent();
            this.gameModes.SelectedIndex = 0;
            
        }

        public bool Initiate()
        {
            this.gameServer = new StandaloneGameServerCLI();

            return true;
        }
     
        public void Run()
        {
            while (this.Created)
            {
                Application.DoEvents();
                this.gameServer.ServerUpdate();
                this.labelClientsConnected.Text = "Clients connected: " + this.gameServer.GetClientsConnectedCount().ToString();
            }
        }
        
        private void button1_serverToggle_Click(object sender, EventArgs e)
        {
            if (this.serverIsRunning)
            {
                this.serverIsRunning = false;
                this.gameServer.ServerStop();
                this.listenPort.Enabled = true;
                this.serverName.Enabled = true;
                this.lanBroadcast.Enabled = true;
                this.serverToggle.Text = "Start server";
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Server terminated!\n");
                this.panel_commands.Visible = false;
            }
            else
            {
                ServerInitDesc desc = new ServerInitDesc();
                //desc.mainOptions.broadcast = this.lanBroadcast.Checked;
                desc.mainOptions.listenPort = (int)this.listenPort.Value;
                desc.mainOptions.serverName = this.serverName.Text;

                if (this.gameServer.ServerInitiate(desc) == DanBiasServerReturn.DanBiasServerReturn_Sucess)
                {
                    this.serverIsRunning = true;

                    GameServerInfo info = this.gameServer.ServerGetInfo();
                    this.Text = this.serverName.Text + " - " + info.serverIp;

                    this.listenPort.Enabled = false;
                    this.serverName.Enabled = false;
                    this.lanBroadcast.Enabled = false;
                    this.serverToggle.Text = "Stop server";
                    this.gameServer.ServerStart();
                    this.panel_commands.Visible = true;
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Server initiated!\n\tListening on port " + this.listenPort.Value.ToString() + "\n\tLocal IP: " + info.serverIp + "\n");
                }
                else
                {
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to initiate the server!");
                }
            }
        }

        private void buttonStartGame_Click(object sender, EventArgs e)
        {
            //this.gameServer.GameSetGameMode(this.gameModes.SelectedText);
            this.gameServer.GameSetGameTime((int)this.timeLimit.Value);
            this.gameServer.GameSetMapName(this.mapName.Text);
            this.gameServer.GameSetMaxClients((int)this.nrOfClients.Value);

            if (!this.gameServer.GameStart( this.forceStart.Checked ))
            {
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to start the game session!\n");
            }
            else
            {
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Game session started!\n");
            }
        }

        private void FormClosingEvent(object sender, FormClosingEventArgs e)
        {
            if (serverIsRunning)
            {
                this.gameServer.ServerStop();
            }
        }
    }
}
