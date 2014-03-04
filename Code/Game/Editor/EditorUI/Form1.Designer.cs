namespace EditorUI
{
    partial class NoEdge
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panel_main = new System.Windows.Forms.Panel();
            this.panel_left = new System.Windows.Forms.Panel();
            this.panel_right = new System.Windows.Forms.Panel();
            this.panel_top = new System.Windows.Forms.Panel();
            this.panel_bottom = new System.Windows.Forms.Panel();
            this.label_EntityName = new System.Windows.Forms.Label();
            this.panel_EntityInfo = new System.Windows.Forms.Panel();
            this.label_position = new System.Windows.Forms.Label();
            this.label_EntityRotation = new System.Windows.Forms.Label();
            this.label_EntityScale = new System.Windows.Forms.Label();
            this.label_EntityLock = new System.Windows.Forms.Label();
            this.imagelist_EntityThumb = new System.Windows.Forms.ImageList(this.components);
            this.treeView_Entities = new System.Windows.Forms.TreeView();
            this.menuTop = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fBXConverterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bulletConverterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel_left.SuspendLayout();
            this.panel_right.SuspendLayout();
            this.panel_top.SuspendLayout();
            this.panel_EntityInfo.SuspendLayout();
            this.menuTop.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel_main
            // 
            this.panel_main.BackColor = System.Drawing.Color.Gray;
            this.panel_main.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel_main.Location = new System.Drawing.Point(193, 65);
            this.panel_main.Name = "panel_main";
            this.panel_main.Size = new System.Drawing.Size(607, 458);
            this.panel_main.TabIndex = 0;
            // 
            // panel_left
            // 
            this.panel_left.BackColor = System.Drawing.Color.DarkGray;
            this.panel_left.Controls.Add(this.treeView_Entities);
            this.panel_left.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel_left.Location = new System.Drawing.Point(0, 65);
            this.panel_left.Name = "panel_left";
            this.panel_left.Size = new System.Drawing.Size(193, 458);
            this.panel_left.TabIndex = 1;
            // 
            // panel_right
            // 
            this.panel_right.BackColor = System.Drawing.Color.DarkGray;
            this.panel_right.Controls.Add(this.panel_EntityInfo);
            this.panel_right.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel_right.Location = new System.Drawing.Point(800, 65);
            this.panel_right.Name = "panel_right";
            this.panel_right.Size = new System.Drawing.Size(183, 458);
            this.panel_right.TabIndex = 2;
            // 
            // panel_top
            // 
            this.panel_top.BackColor = System.Drawing.Color.Silver;
            this.panel_top.Controls.Add(this.menuTop);
            this.panel_top.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel_top.Location = new System.Drawing.Point(0, 0);
            this.panel_top.Name = "panel_top";
            this.panel_top.Size = new System.Drawing.Size(983, 65);
            this.panel_top.TabIndex = 3;
            // 
            // panel_bottom
            // 
            this.panel_bottom.BackColor = System.Drawing.Color.Silver;
            this.panel_bottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel_bottom.Location = new System.Drawing.Point(0, 523);
            this.panel_bottom.Name = "panel_bottom";
            this.panel_bottom.Size = new System.Drawing.Size(983, 60);
            this.panel_bottom.TabIndex = 4;
            // 
            // label_EntityName
            // 
            this.label_EntityName.AutoSize = true;
            this.label_EntityName.Location = new System.Drawing.Point(51, 18);
            this.label_EntityName.Name = "label_EntityName";
            this.label_EntityName.Size = new System.Drawing.Size(61, 13);
            this.label_EntityName.TabIndex = 0;
            this.label_EntityName.Text = "EntityName";
            // 
            // panel_EntityInfo
            // 
            this.panel_EntityInfo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.panel_EntityInfo.Controls.Add(this.label_EntityLock);
            this.panel_EntityInfo.Controls.Add(this.label_EntityScale);
            this.panel_EntityInfo.Controls.Add(this.label_EntityRotation);
            this.panel_EntityInfo.Controls.Add(this.label_position);
            this.panel_EntityInfo.Controls.Add(this.label_EntityName);
            this.panel_EntityInfo.Location = new System.Drawing.Point(6, 6);
            this.panel_EntityInfo.Name = "panel_EntityInfo";
            this.panel_EntityInfo.Size = new System.Drawing.Size(174, 135);
            this.panel_EntityInfo.TabIndex = 1;
            this.panel_EntityInfo.Visible = false;
            // 
            // label_position
            // 
            this.label_position.AutoSize = true;
            this.label_position.Location = new System.Drawing.Point(26, 41);
            this.label_position.Name = "label_position";
            this.label_position.Size = new System.Drawing.Size(44, 13);
            this.label_position.TabIndex = 0;
            this.label_position.Text = "Position";
            // 
            // label_EntityRotation
            // 
            this.label_EntityRotation.AutoSize = true;
            this.label_EntityRotation.Location = new System.Drawing.Point(26, 63);
            this.label_EntityRotation.Name = "label_EntityRotation";
            this.label_EntityRotation.Size = new System.Drawing.Size(39, 13);
            this.label_EntityRotation.TabIndex = 0;
            this.label_EntityRotation.Text = "Rotate";
            // 
            // label_EntityScale
            // 
            this.label_EntityScale.AutoSize = true;
            this.label_EntityScale.Location = new System.Drawing.Point(26, 84);
            this.label_EntityScale.Name = "label_EntityScale";
            this.label_EntityScale.Size = new System.Drawing.Size(34, 13);
            this.label_EntityScale.TabIndex = 0;
            this.label_EntityScale.Text = "Scale";
            // 
            // label_EntityLock
            // 
            this.label_EntityLock.AutoSize = true;
            this.label_EntityLock.Location = new System.Drawing.Point(26, 103);
            this.label_EntityLock.Name = "label_EntityLock";
            this.label_EntityLock.Size = new System.Drawing.Size(43, 13);
            this.label_EntityLock.TabIndex = 0;
            this.label_EntityLock.Text = "Locked";
            // 
            // imagelist_EntityThumb
            // 
            this.imagelist_EntityThumb.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imagelist_EntityThumb.ImageSize = new System.Drawing.Size(32, 32);
            this.imagelist_EntityThumb.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // treeView_Entities
            // 
            this.treeView_Entities.ImageIndex = 0;
            this.treeView_Entities.ImageList = this.imagelist_EntityThumb;
            this.treeView_Entities.Location = new System.Drawing.Point(4, 6);
            this.treeView_Entities.Name = "treeView_Entities";
            this.treeView_Entities.SelectedImageIndex = 0;
            this.treeView_Entities.ShowLines = false;
            this.treeView_Entities.Size = new System.Drawing.Size(184, 446);
            this.treeView_Entities.TabIndex = 0;
            this.treeView_Entities.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView_Entities_AfterSelect);
            // 
            // menuTop
            // 
            this.menuTop.BackColor = System.Drawing.Color.Transparent;
            this.menuTop.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuTop.Location = new System.Drawing.Point(0, 0);
            this.menuTop.Name = "menuTop";
            this.menuTop.Size = new System.Drawing.Size(983, 24);
            this.menuTop.TabIndex = 0;
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.loadToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fBXConverterToolStripMenuItem,
            this.bulletConverterToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // fBXConverterToolStripMenuItem
            // 
            this.fBXConverterToolStripMenuItem.Name = "fBXConverterToolStripMenuItem";
            this.fBXConverterToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.fBXConverterToolStripMenuItem.Text = "FBX converter";
            this.fBXConverterToolStripMenuItem.Click += new System.EventHandler(this.fBXConverterToolStripMenuItem_Click);
            // 
            // bulletConverterToolStripMenuItem
            // 
            this.bulletConverterToolStripMenuItem.Name = "bulletConverterToolStripMenuItem";
            this.bulletConverterToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.bulletConverterToolStripMenuItem.Text = "Bullet converter";
            this.bulletConverterToolStripMenuItem.Click += new System.EventHandler(this.bulletConverterToolStripMenuItem_Click);
            // 
            // NoEdge
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(983, 583);
            this.Controls.Add(this.panel_main);
            this.Controls.Add(this.panel_right);
            this.Controls.Add(this.panel_left);
            this.Controls.Add(this.panel_top);
            this.Controls.Add(this.panel_bottom);
            this.MainMenuStrip = this.menuTop;
            this.Name = "NoEdge";
            this.Text = "No Edge editor";
            this.panel_left.ResumeLayout(false);
            this.panel_right.ResumeLayout(false);
            this.panel_top.ResumeLayout(false);
            this.panel_top.PerformLayout();
            this.panel_EntityInfo.ResumeLayout(false);
            this.panel_EntityInfo.PerformLayout();
            this.menuTop.ResumeLayout(false);
            this.menuTop.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel_main;
        private System.Windows.Forms.Panel panel_left;
        private System.Windows.Forms.Panel panel_right;
        private System.Windows.Forms.Panel panel_top;
        private System.Windows.Forms.Panel panel_bottom;
        private System.Windows.Forms.Panel panel_EntityInfo;
        private System.Windows.Forms.Label label_EntityLock;
        private System.Windows.Forms.Label label_EntityScale;
        private System.Windows.Forms.Label label_EntityRotation;
        private System.Windows.Forms.Label label_position;
        private System.Windows.Forms.Label label_EntityName;
        private System.Windows.Forms.ImageList imagelist_EntityThumb;
        private System.Windows.Forms.TreeView treeView_Entities;
        private System.Windows.Forms.MenuStrip menuTop;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fBXConverterToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bulletConverterToolStripMenuItem;
    }
}

