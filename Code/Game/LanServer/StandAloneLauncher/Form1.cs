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
using System.IO;

namespace StandAloneLauncher
{
    public partial class Form1 : Form
    {
        System.Windows.Interop.StandaloneGameServerCLI gameServer;
        bool serverIsRunning = false;
        bool gameIsStarted = false;

        public Form1()
        {
            InitializeComponent();


            string[] maps = Directory.GetFiles("..\\Content\\Worlds\\");
            for (int i = 0; i < maps.Length; i++)
            {
                string temp = maps[i].Split('\\').Last();
                string type = temp.Split('.').Last();
                if (type == "bias")
                {
                    this.mapName.Items.Add(temp);
                }
            }

            this.gameModes.SelectedIndex = 0;
            this.mapName.SelectedIndex = 0;
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
                if (gameIsStarted)
                {
                    //this.gameServer.GameStop();

                    this.gameIsStarted = false;
                    this.buttonStartGame.Text = "Start Game";
                    this.mapName.Enabled = true;
                    this.nrOfClients.Enabled = true;
                    this.gameModes.Enabled = true;
                    this.timeLimit.Enabled = true;
                    this.forceStart.Enabled = true;
                }

                this.serverIsRunning = false;
                this.gameServer.ServerStop();
                this.listenPort.Enabled = true;
                this.serverName.Enabled = true;
                this.lanBroadcast.Enabled = true;
                this.serverToggle.Text = "Start server";
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Server terminated!\n");
                this.panel_commands.Visible = false;
                //this.panelServerCommands.Visible = false;
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
                    //this.panelServerCommands.Visible = true;
                }
                else
                {
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to initiate the server!\n");
                }
            }
        }

        private void buttonStartGame_Click(object sender, EventArgs e)
        {
            if (!gameIsStarted)
            {
                //this.gameServer.GameSetGameMode(this.gameModes.SelectedText);
                this.gameServer.GameSetGameTime((int)this.timeLimit.Value);



                this.gameServer.GameSetMapName(this.mapName.Text);
                this.gameServer.GameSetMaxClients((int)this.nrOfClients.Value);

                if (!(gameIsStarted = this.gameServer.GameStart(this.forceStart.Checked)))
                {
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to start the game session!\n");
                }
                else
                {
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Game session started!\n");
                    this.buttonStartGame.Text = "Stop Game";

                    this.mapName.Enabled = false;
                    this.nrOfClients.Enabled = false;
                    this.gameModes.Enabled = false;
                    this.timeLimit.Enabled = false;
                    this.forceStart.Enabled = false;
                }
            }
            else
            {
                //this.gameServer.GameStop();

                this.gameIsStarted = false;
                this.buttonStartGame.Text = "Start Game";
                this.mapName.Enabled        = true;
                this.nrOfClients.Enabled    = true;
                this.gameModes.Enabled      = true;
                this.timeLimit.Enabled      = true;
                this.forceStart.Enabled     = true;
            }
        }

        private void FormClosingEvent(object sender, FormClosingEventArgs e)
        {
            if (serverIsRunning)
            {
                this.gameServer.ServerStop();
            }
        }

        private void buttonAddNewDataField_Click(object sender, EventArgs e)
        {
            this.dataProtocolFields.RowCount++;
            this.dataProtocolFields.SetRow(this.buttonsAtBottom, this.dataProtocolFields.RowCount - 1);

            Panel p = new Panel();
            p = this.panel2;

            this.dataProtocolFields.RowStyles.Add(new RowStyle(SizeType.Absolute, 27));

        }

        private void mapName_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ( this.panel_commands.Visible && (this.mapName.SelectedItem.ToString() == "Set directory"))
            {
                FolderBrowserDialog f = new FolderBrowserDialog();
                DialogResult r = f.ShowDialog();
                if (r == System.Windows.Forms.DialogResult.OK)
                {
                    this.mapName.Items.Clear();
                    this.mapName.Items.Add("Set directory");
                    string[] maps = Directory.GetFiles(f.SelectedPath);
                    for (int i = 0; i < maps.Length; i++)
                    {
                        string temp = maps[i].Split('\\').Last();
                        string type = temp.Split('.').Last();
                        if (type == "bias")
                        {
                            this.mapName.Items.Add(temp);
                        }
                    }
                }
            }
        }
    }
}
