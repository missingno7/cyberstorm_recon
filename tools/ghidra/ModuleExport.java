// Ghidra headless post-script: create the seeded functions, name them, export pseudo-C per module.
// args: seeds.tsv (va_hex, size, name, module_start_hex) and an output directory.
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

public class ModuleExport extends GhidraScript {
    public void run() throws Exception {
        String[] a = getScriptArgs();
        File outDir = new File(a[1]); outDir.mkdirs();
        List<String[]> rows = new ArrayList<>();
        for (String line : java.nio.file.Files.readAllLines(new File(a[0]).toPath(), StandardCharsets.UTF_8)) {
            String[] x = line.split("\t", -1);
            if (x.length == 4) rows.add(x);
        }
        // pass 1: create and name every function so calls decompile with names
        for (String[] x : rows) {
            Address ad = toAddr(Long.parseLong(x[0], 16));
            Function f = getFunctionAt(ad);
            if (f == null) { disassemble(ad); f = createFunction(ad, x[2]); }
            if (f != null && !x[2].equals(f.getName())) {
                try { f.setName(x[2], SourceType.USER_DEFINED); } catch (Exception ignored) {}
            }
        }
        Map<String, PrintWriter> writers = new HashMap<>();
        DecompInterface d = new DecompInterface(); d.openProgram(currentProgram);
        int n = 0;
        try {
            for (String[] x : rows) {
                if (x[3].isEmpty()) continue;
                Function f = getFunctionAt(toAddr(Long.parseLong(x[0], 16)));
                PrintWriter w = writers.get(x[3]);
                if (w == null) {
                    w = new PrintWriter(new OutputStreamWriter(new FileOutputStream(new File(outDir, x[3] + ".c")), StandardCharsets.UTF_8));
                    writers.put(x[3], w);
                }
                w.printf("/* VA=%s size=%s name=%s */%n", x[0], x[1], x[2]);
                if (f == null) { w.println("/* no function */"); continue; }
                DecompileResults r = d.decompileFunction(f, 60, monitor);
                if (r.decompileCompleted()) w.println(r.getDecompiledFunction().getC());
                else w.printf("/* DECOMPILE FAILED: %s */%n", r.getErrorMessage());
                n++;
            }
        } finally {
            d.dispose(); for (PrintWriter w : writers.values()) w.close();
        }
        println("ModuleExport: exported " + n + " functions into " + writers.size() + " module files");
    }
}
