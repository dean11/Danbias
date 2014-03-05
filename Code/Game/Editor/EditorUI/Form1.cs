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
using System.Windows.Interop.EnumBridge;
using System.Threading;
using System.Timers;
using System.IO;
using System.Diagnostics;


namespace EditorUI
{
    public struct EntityData
    {
        public NoEdgeType type;
        public int subtype;
        public string text;
        //...

        public EntityData(NoEdgeType type, int subtype, string text) 
        {
            this.type = type;
            this.subtype = subtype; 
            this.text = text; 
        }
    }
    public partial class NoEdge : Form
    {
        EditorCLIWrapper editor;
        List<GamePlanet> planets;
        List<GameEntity> entities;

        public NoEdge()
        {
            InitializeComponent();

            this.ResizeEnd += NoEdge_ResizeEnd;

            this.editor = new EditorCLIWrapper();
            this.entities = new List<GameEntity>();
            this.planets = new List<GamePlanet>();
        }
        void NoEdge_ResizeEnd(object sender, EventArgs e)
        {
            this.editor.OnResize(this.panel_main.Width, this.panel_main.Height);
        }

        public bool Initiate()
        {
            this.InitiateEntityList();

            InitDesc desc;
            desc.mainOptions.height = this.panel_main.Height;
            desc.mainOptions.width = this.panel_main.Width;
            desc.mainOptions.renderWindow = this.panel_main.Handle;

            if (this.editor.InitiateCore(desc) == EditorReturnCode.EditorReturnCode_Error)
            {
                MessageBox.Show("Failed to initiate No Edge core");
                return false;
            }
            return true;
        }
        private void AddTypePlanet()
        {
            List<TreeNode> special = new List<TreeNode>();
            TreeNode node;
            EntityData data;

            data = new EntityData(NoEdgeType.NoEdgeType_World, -1, "Sphere planet");
            node = new TreeNode(data.text);
            node.Tag = data;
            special.Add(node);

            TreeNode temp = new TreeNode("Planet", special.ToArray());
            temp.ImageIndex = -1;
            temp.StateImageIndex = -1;
            this.treeView_Entities.Nodes.Add(temp);
        }
        private void AddTypeBuilding()
        {
            List<TreeNode> buildings = new List<TreeNode>();
            TreeNode node;
            EntityData data;

            data = new EntityData(NoEdgeType.NoEdgeType_Building, (int)NoEdgeType_Buildings.CorporateBuilding, NoEdgeType_Buildings.CorporateBuilding.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            buildings.Add(node);

            //data = new EntityData(NoEdgeType.NoEdgeType_Building, (int)NoEdgeType_Buildings.CrystalFormation, NoEdgeType_Buildings.CrystalFormation.ToString());
            //node = new TreeNode(data.text);
            //node.Tag = data;
            //buildings.Add(node);

            TreeNode temp = new TreeNode("Building", buildings.ToArray());
            temp.ImageIndex = -1;
            temp.StateImageIndex = -1;
            this.treeView_Entities.Nodes.Add(temp);
        }
        private void AddTypeInteractive()
        {
            List<TreeNode> interactive = new List<TreeNode>();
            TreeNode node;
            EntityData data;

            data = new EntityData(NoEdgeType.NoEdgeType_Interactive, (int)NoEdgeType_Interactive.JumpPad, NoEdgeType_Interactive.JumpPad.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            interactive.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Interactive, (int)NoEdgeType_Interactive.JumpPad, NoEdgeType_Interactive.JumpPad.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            interactive.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Interactive, (int)NoEdgeType_Interactive.SpawnPoint, NoEdgeType_Interactive.SpawnPoint.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            interactive.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Interactive, (int)NoEdgeType_Interactive.ExplosiveBox, NoEdgeType_Interactive.ExplosiveBox.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            interactive.Add(node);
           

            TreeNode head = new TreeNode("Interactive", interactive.ToArray());
            head.ImageIndex = -1;
            head.StateImageIndex = -1;
            this.treeView_Entities.Nodes.Add(head);
        }
        private void AddTypeProjectile()
        {
            List<TreeNode> projectiles = new List<TreeNode>();
            TreeNode node;
            EntityData data;

            data = new EntityData(NoEdgeType.NoEdgeType_Projectile, (int)NoEdgeType_Projectiles.CrystalShard, NoEdgeType_Projectiles.CrystalShard.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            projectiles.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Projectile, (int)NoEdgeType_Projectiles.Spike, NoEdgeType_Projectiles.Spike.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            projectiles.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Projectile, (int)NoEdgeType_Projectiles.SpikeBox, NoEdgeType_Projectiles.SpikeBox.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            projectiles.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Projectile, (int)NoEdgeType_Projectiles.StandardBox, NoEdgeType_Projectiles.StandardBox.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            projectiles.Add(node);

            data = new EntityData(NoEdgeType.NoEdgeType_Projectile, (int)NoEdgeType_Projectiles.Stone, NoEdgeType_Projectiles.Stone.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            projectiles.Add(node);

            TreeNode temp = new TreeNode("Projectiles", projectiles.ToArray());
            temp.ImageIndex = -1;
            temp.StateImageIndex = -1;
            this.treeView_Entities.Nodes.Add(temp);
        }
        private void AddTypeLight()
        {
            List<TreeNode> lights = new List<TreeNode>();
            TreeNode node;
            EntityData data;

            data = new EntityData(NoEdgeType.NoEdgeType_Light, (int)NoEdgeType_Light.PointLight, NoEdgeType_Light.PointLight.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            lights.Add(node);

            TreeNode temp = new TreeNode("Light", lights.ToArray());
            temp.ImageIndex = -1;
            temp.StateImageIndex = -1;
            this.treeView_Entities.Nodes.Add(temp);
        }
        private void AddTypeHazardous()
        {
            List<TreeNode> hazard = new List<TreeNode>();
            TreeNode node;
            EntityData data;

            data = new EntityData(NoEdgeType.NoEdgeType_HazardEnv, (int)NoEdgeType_HazardEnv.CrystalFormation1, NoEdgeType_HazardEnv.CrystalFormation1.ToString());
            node = new TreeNode(data.text);
            node.Tag = data;
            hazard.Add(node);
            
            TreeNode temp = new TreeNode("Hazardous Environment", hazard.ToArray());
            temp.ImageIndex = -1;
            temp.StateImageIndex = -1;
            this.treeView_Entities.Nodes.Add(temp);
        }
        private void AddTypePickup()
        {
           List<TreeNode> pick = new List<TreeNode>();
           TreeNode node;
           EntityData data;

           data = new EntityData(NoEdgeType.NoEdgeType_Pickup, (int)NoEdgeType_Pickup.HealthPackMedium, NoEdgeType_Pickup.HealthPackMedium.ToString());
           node = new TreeNode(data.text);
           node.Tag = data;
           pick.Add(node);

           TreeNode temp = new TreeNode("Light", pick.ToArray());
           temp.ImageIndex = -1;
           temp.StateImageIndex = -1;
           this.treeView_Entities.Nodes.Add(temp);
        }
        private void InitiateEntityList()
        {
            string [] thumbs = Directory.GetFiles("..\\Content\\Textures\\edit");

            for (int i = 0; i < thumbs.Length; i++)
			{
                string end = thumbs[i].Substring(thumbs[i].Length - 3, 3);
                if ( end == "png" || end == "jpg" )
                {
                    Image t = Image.FromFile(thumbs[i]);
                    this.imagelist_EntityThumb.Images.Add(t);
                }
			}
            this.treeView_Entities.Indent = 10;

            AddTypePlanet();
            AddTypeBuilding();
            AddTypeInteractive();
            AddTypeProjectile();
            AddTypeLight();
        }
        public void Run()
        {
            while (this.Visible)
            {
                Application.DoEvents();

                UpdateCore();
            }

            try
            {
                this.editor.Release();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
            finally
            {
                this.Dispose();
            }
        }

        public void UpdateCore()
        {
            this.editor.Frame();
        }

        private void treeView_Entities_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Action == TreeViewAction.Unknown)
                return;

           //Extract node infor
            if (e.Node.Tag == null) return;

            object o = ((EntityData)e.Node.Tag);
            if (o != null)
            {
                EntityData data = (EntityData)o;
                switch (data.type)
                {
                    case NoEdgeType.NoEdgeType_Building:
                        {
                            GameEntity entity = this.editor.CreateEntity((NoEdgeType_Buildings)data.subtype);
                            this.entities.Add(entity);
                        }
                        break;
                    case NoEdgeType.NoEdgeType_Interactive:
                        {
                            GameEntity entity = this.editor.CreateEntity((NoEdgeType_Interactive)data.subtype);
                            this.entities.Add(entity);
                        }
                        break;
                    case NoEdgeType.NoEdgeType_Light:
                        {
                            GameEntity entity = this.editor.CreateEntity((NoEdgeType_Light)data.subtype);
                            this.entities.Add(entity);
                        }
                        break;
                    case NoEdgeType.NoEdgeType_Projectile:
                        {
                            GameEntity entity = this.editor.CreateEntity((NoEdgeType_Projectiles)data.subtype);
                            this.entities.Add(entity);
                        }
                        break;
                    case NoEdgeType.NoEdgeType_World:
                        {
                            GamePlanet world = this.editor.CreatePlanet();
                            this.planets.Add(world);
                        }
                        break;
                }
            }
           
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Reset
            DialogResult r = MessageBox.Show("Are you sure you want to discard\nunsaved changes and create a new \nworld?", "Warning", MessageBoxButtons.YesNo);
            if (r == System.Windows.Forms.DialogResult.Yes)
            {
                //this.editor.UnloadMap();
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog fd = new SaveFileDialog();
            fd.Filter = "NoEdge Level file (*.bias)|*.bias";
            fd.AddExtension = true;
            if (fd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                //this.editor.Export(fd.FileName);
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = "NoEdge Level file (*.*)|*.bias";
            if (fd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (fd.CheckFileExists)
                {
                    //this.editor.LoadMap(fd.FileName);
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Visible = false;
        }

        private void fBXConverterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Use ProcessStartInfo class
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = false;
            startInfo.UseShellExecute = false;
            startInfo.FileName = "..\\Utilities\\fbxconv.exe";
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;

            OpenFileDialog t = new OpenFileDialog();
            t.Filter = "FBX (*.*)|*.fbx";
            t.InitialDirectory = Directory.GetCurrentDirectory();
            if (t.ShowDialog() != System.Windows.Forms.DialogResult.OK) return;

            string of = t.SafeFileName.Replace(".fbx", ".dan");
            string output = "-o " + "..\\Content\\Models\\" + of;

            startInfo.Arguments = t.FileName + " " + output;

            try
            {
                using (Process exeProcess = Process.Start(startInfo))
                {
                    exeProcess.WaitForExit();
                }
            }
            catch (Exception a)
            {
                MessageBox.Show(a.Message);
            }
        }

        private void bulletConverterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Use ProcessStartInfo class
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.CreateNoWindow = false;
            startInfo.UseShellExecute = false;
            startInfo.FileName = "..\\Utilities\\bulletconv.exe";
            startInfo.WindowStyle = ProcessWindowStyle.Hidden;

            OpenFileDialog t = new OpenFileDialog();
            t.Filter = "dan (*.*)|*.dan";
            t.InitialDirectory = Directory.GetCurrentDirectory();
            if (t.ShowDialog() != System.Windows.Forms.DialogResult.OK) 
                return;

            string of = t.SafeFileName.Replace(".dan", ".bullet");
            string output = "..\\Content\\Worlds\\cgf\\" + of;

            startInfo.Arguments = t.FileName + " " + output;
            try
            {
                using (Process exeProcess = Process.Start(startInfo))
                {
                    exeProcess.WaitForExit();
                }
            }
            catch (Exception a)
            {
                MessageBox.Show(a.Message);
            }
        }
    }
}
