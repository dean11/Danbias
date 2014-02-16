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

namespace StandAloneLauncher
{
    public partial class Form1 : Form
    {
        System.Windows.Interop.StandaloneGameServerCLI gameServer;
        bool serverIsRunning = false;

        public Form1()
        {
            InitializeComponent();

            
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

                //Do some stuff
                this.gameServer.ServerUpdate();
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
            }
            else
            {
                this.serverIsRunning = true;
                ServerInitDesc desc = new ServerInitDesc();
                desc.broadcast = this.lanBroadcast.Checked;
                desc.listenPort = (int)this.listenPort.Value;
                desc.serverName = this.serverName.Text;

                if (this.gameServer.ServerInitiate(desc) == DanBiasServerReturn.DanBiasServerReturn_Sucess)
                {
                    this.listenPort.Enabled = false;
                    this.serverName.Enabled = false;
                    this.lanBroadcast.Enabled = false;
                    this.serverToggle.Text = "Stop server";
                    this.gameServer.ServerStart();
                    this.clientInfoBox.Items.Add((Object)"Server initiated!");
                }
            }
        }
    }
}
